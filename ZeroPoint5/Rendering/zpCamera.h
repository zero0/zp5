#pragma once
#ifndef ZP_CAMERA_H
#define ZP_CAMERA_H

enum zpCameraProjection
{
    ZP_CAMERA_PROJECTION_ORTHO,
    ZP_CAMERA_PROJECTION_ORTHO_CENTERED,
    ZP_CAMERA_PROJECTION_ORTHO_OFFSET,

    ZP_CAMERA_PROJECTION_PERSPECTIVE,
    ZP_CAMERA_PROJECTION_PERSPECTIVE_OFFSET,

    zpCameraProjection_Count,
    zpCameraProjection_Force32 = ZP_FORECE_32BIT
};

enum zpCameraClearMode
{
    ZP_CAMERA_CLEAR_MODE_NONE =                0,
    ZP_CAMERA_CLEAR_MODE_COLOR =               ( 1 << 0 ),
    ZP_CAMERA_CLEAR_MODE_DEPTH =               ( 1 << 1 ),
    ZP_CAMERA_CLEAR_MODE_STENCIL =             ( 1 << 2 ),
    ZP_CAMERA_CLEAR_MODE_BEFORE_RENDER =       ( 1 << 3 ),

    ZP_CAMERA_CLEAR_MODE_DEFAULT = ZP_CAMERA_CLEAR_MODE_COLOR | ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL | ZP_CAMERA_CLEAR_MODE_BEFORE_RENDER,

    zpCameraClearMode_Force32 = ZP_FORECE_32BIT
};

struct zpCamera
{
    zpVector4fData position;
    zpVector4fData up;
    zpVector4fData forward;
    zpVector4fData lookAt;

    zp_uint viewportIndex;
    zp_uint layerMask;
    zp_uint flags;
    zp_int order;

    zpCameraProjection projectionType;
    zpCameraClearMode clearMode;

    zpRecti orthoRect;
    zpRecti clipRect;
    zpViewport viewport;
    zpVector2f viewOffset;

    zpColorf clearColor;
    zp_float clearDepth;
    zp_uint clearStencil;

    zpFrustum frustum;

    zp_float zNear;
    zp_float zFar;
    zp_float fovy;
    zp_float aspectRatio;

    zpMatrix4fData viewMatrix;
    zpMatrix4fData projectionMatrix;
    zpMatrix4fData viewProjectionMatrix;
    zpMatrix4fData invViewProjectionMatrix;
};

struct zpCameraInstance
{
    zp_size_t refCount;
    zp_hash64 instanceId;
    zpCamera camera;
};

class zpCameraHandle
{
public:
    zpCameraHandle();
    zpCameraHandle( const zpCameraHandle& other );
    zpCameraHandle( zpCameraHandle&& other );
    ~zpCameraHandle();

    zpCameraHandle& operator=( const zpCameraHandle& other );
    zpCameraHandle& operator=( zpCameraHandle&& other );

    const zpCamera* operator->() const;
    zpCamera* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpCameraInstance* objectInstance );

    zpCameraInstance* m_cameraInstance;
    zp_hash64 m_instanceId;

    friend class zpCameraManager;
};

class zpCameraManager
{
public:
    zpCameraManager();
    ~zpCameraManager();

    void setup();
    void teardown();

    void update( zp_float dt, zp_float rt );

    void createCamera( zpCameraHandle& handle );

    zp_bool findCameraForLayer( zp_uint layerIndex, zpCameraHandle& handle ) const;

    void garbageCollect();

private:
    zpVector< zpCameraInstance* > m_cameraInstances;
    zp_hash64 m_newCameraInstanceId;
};

#endif // ZP_CAMERA_H
