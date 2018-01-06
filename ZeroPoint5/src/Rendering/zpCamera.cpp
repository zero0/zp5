#include "Rendering/zpRendering.h"
#include <new>

enum zpCameraFlag
{
    ZP_CAMERA_FLAG_ENABLED =            1 << 0,
    ZP_CAMERA_FLAG_VIEW_DIRTY =         1 << 1,
    ZP_CAMERA_FLAG_PROJECTION_DIRTY =   1 << 2,

    zpCameraFlag_Count,
    zpCameraFlag_Force32 = ZP_FORECE_32BIT
};

const zp_hash64 ZP_CAMERA_ID_INVALID = (zp_hash64)-1;
const zp_hash64 ZP_CAMERA_ID_EMPTY = 0;

//
//
//

zpCameraHandle::zpCameraHandle()
    : m_instanceId( ZP_CAMERA_ID_INVALID )
    , m_cameraInstance( ZP_NULL )
{
}

zpCameraHandle::zpCameraHandle( const zpCameraHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_cameraInstance( other.m_cameraInstance )
{
    addRef();
}

zpCameraHandle::zpCameraHandle( zpCameraHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_cameraInstance( other.m_cameraInstance )
{
    addRef();
    other.release();
}

zpCameraHandle::~zpCameraHandle()
{
    release();
}

zpCameraHandle& zpCameraHandle::operator=( const zpCameraHandle& other )
{
    set( other.m_instanceId, other.m_cameraInstance );

    return *this;
}
zpCameraHandle& zpCameraHandle::operator=( zpCameraHandle&& other )
{
    set( other.m_instanceId, other.m_cameraInstance );

    other.release();

    return *this;
}

const zpCamera* zpCameraHandle::operator->() const
{
    return isValid() ? &m_cameraInstance->camera : ZP_NULL;
}
zpCamera* zpCameraHandle::operator->()
{
    return isValid() ? &m_cameraInstance->camera : ZP_NULL;
}

zp_bool zpCameraHandle::isValid() const
{
    return m_cameraInstance != ZP_NULL && m_cameraInstance->instanceId == m_instanceId;
}

void zpCameraHandle::release()
{
    releaseRef();

    m_instanceId = ZP_CAMERA_ID_INVALID;
    m_cameraInstance = ZP_NULL;
}

void zpCameraHandle::addRef()
{
    if( isValid() )
    {
        ++m_cameraInstance->refCount;
    }
}

void zpCameraHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_cameraInstance->refCount != 0, "" );
        --m_cameraInstance->refCount;
    }
}

void zpCameraHandle::set( zp_hash64 instanceId, zpCameraInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_cameraInstance = objectInstance;

    addRef();
}

//
//
//

zpCameraManager::zpCameraManager()
    : m_cameraInstances( 4 )
    , m_newCameraInstanceId( ZP_CAMERA_ID_EMPTY )
{
}
zpCameraManager::~zpCameraManager()
{
}

void zpCameraManager::setup()
{
}

void zpCameraManager::teardown()
{
}

void zpCameraManager::update( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();
   
    zpCameraInstance** b = m_cameraInstances.begin();
    zpCameraInstance** e = m_cameraInstances.end();
    for( ; b != e; ++b )
    {
        zpCameraInstance* c = (*b);
        if( c->refCount )
        {
            zpCamera* cam = &c->camera;

            if( cam->flags & ZP_CAMERA_FLAG_ENABLED )
            {
                zp_bool isViewProjectionDirty = false;

                if( cam->flags & ZP_CAMERA_FLAG_VIEW_DIRTY )
                {
                    ZP_ALIGN16 zpVector4fData p = cam->position;
                    ZP_ALIGN16 zpVector4fData f = cam->forward;
                    ZP_ALIGN16 zpVector4fData u = cam->up;

                    zpVector4f eye = zpMath::Vector4Load4( p.m );
                    zpVector4f dir = zpMath::Vector4Load4( f.m );
                    zpVector4f up  = zpMath::Vector4Load4( u.m );

                    zpMatrix4f view = zpMath::LookAtLH( eye, dir, up );
                    
                    ZP_ALIGN16 zpMatrix4fData mat;
                    zpMath::MatrixStore4( view, mat.m );
                    cam->viewMatrix = mat;

                    zpMath::FrustrumSetLookTo( cam->frustum, cam->position, cam->forward, cam->up, cam->aspectRatio, cam->fovy, cam->zNear, cam->zFar );

                    isViewProjectionDirty = true;
                    cam->flags &= ~ZP_CAMERA_FLAG_VIEW_DIRTY;
                }

                if( cam->flags & ZP_CAMERA_FLAG_PROJECTION_DIRTY )
                {
                    zpMatrix4f projection;

                    switch( cam->projectionType )
                    {
                    case ZP_CAMERA_PROJECTION_ORTHO:
                        {
                            zp_float l = static_cast<zp_float>( cam->orthoRect.x  );
                            zp_float r = static_cast<zp_float>( cam->orthoRect.x + cam->orthoRect.width  );
                            zp_float t = static_cast<zp_float>( cam->orthoRect.y );
                            zp_float b = static_cast<zp_float>( cam->orthoRect.y + cam->orthoRect.height );
                            zp_float n = cam->zNear;
                            zp_float f = cam->zFar;

                            projection = zpMath::OrthoLH( l, r, t, b, n, f );
                        } break;

                    case ZP_CAMERA_PROJECTION_ORTHO_CENTERED:
                        {
                            zp_float l = static_cast<zp_float>( cam->orthoRect.width  ) * -0.5f;
                            zp_float r = static_cast<zp_float>( cam->orthoRect.width  ) *  0.5f;
                            zp_float t = static_cast<zp_float>( cam->orthoRect.height ) *  0.5f;
                            zp_float b = static_cast<zp_float>( cam->orthoRect.height ) * -0.5f;
                            zp_float n = cam->zNear;
                            zp_float f = cam->zFar;

                            projection = zpMath::OrthoLH( l, r, t, b, n, f );
                        } break;

                    case ZP_CAMERA_PROJECTION_PERSPECTIVE:
                        {
                            zp_float fovy =         cam->fovy;
                            zp_float aspectRatio =  cam->aspectRatio;
                            zp_float n =            cam->zNear;
                            zp_float f =            cam->zFar;

                            projection = zpMath::PerspectiveLH( fovy, aspectRatio, n, f );
                        } break;

                    default:
                        ZP_INVALID_CODE_PATH();
                        break;
                    }

                    ZP_ALIGN16 zpMatrix4fData mat;
                    zpMath::MatrixStore4( projection, mat.m );
                    cam->projectionMatrix = mat;

                    isViewProjectionDirty = true;
                    cam->flags &= ~ZP_CAMERA_FLAG_PROJECTION_DIRTY;
                }

                if( isViewProjectionDirty )
                {
                    ZP_ALIGN16 zpMatrix4fData vMat = cam->viewMatrix;
                    ZP_ALIGN16 zpMatrix4fData pMat = cam->projectionMatrix;
                    ZP_ALIGN16 zpMatrix4fData vpMat;

                    zpMatrix4f view = zpMath::MatrixLoad4( vMat.m );
                    zpMatrix4f projection = zpMath::MatrixLoad4( pMat.m );

                    zpMatrix4f viewProjection = zpMath::MatrixMul( view, projection );
                    zpMatrix4f invViewProjection = zpMath::MatrixIdentity();

                    zpMath::MatrixStore4( viewProjection, vpMat.m );
                    cam->viewProjectionMatrix = vpMat;

                    zpMath::MatrixStore4( invViewProjection, vpMat.m );
                    cam->invViewProjectionMatrix = vpMat;
                }
            }
        }
    }
}

void zpCameraManager::createCamera( zpCameraHandle& handle )
{
    zp_hash64 instanceId = ++m_newCameraInstanceId;

    zpCameraInstance* instance = static_cast<zpCameraInstance*>( g_globalAllocator.allocate( sizeof( zpCameraInstance ) ) );

    new ( &instance->camera ) zpCamera();

    instance->instanceId = instanceId;
    instance->refCount = 0;

    handle.set( instanceId, instance );

    m_cameraInstances.pushBack( instance );
}

zp_bool zpCameraManager::findCameraForLayer( zp_uint layerIndex, zpCameraHandle& handle ) const
{
    zp_bool found = false;

    for( zp_size_t i = 0, imax = m_cameraInstances.size(); i != imax; ++i )
    {
        zpCameraInstance* b = m_cameraInstances[ i ];
        if( b->refCount && ( b->camera.layerMask & ( 1 << layerIndex ) ))
        {
            handle.set( b->instanceId, b );
            found = true;
            break;
        }
    }

    return found;
}

void zpCameraManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_cameraInstances.size(); i != imax; ++i )
    {
        zpCameraInstance* b = m_cameraInstances[ i ];
        if( b->refCount == 0 )
        {
            b->~zpCameraInstance();

            g_globalAllocator.free( b );

            m_cameraInstances.erase( i );

            --i;
            --imax;
        }
    }
}
