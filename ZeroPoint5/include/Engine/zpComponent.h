#pragma once
#ifndef ZP_COMPONENT_H
#define ZP_COMPONENT_H

enum zpComponentFlags : zp_flag32
{
    ZP_COMPONENT_FLAG_CREATED = 0,
    ZP_COMPONENT_FLAG_STARTED,
    ZP_COMPONENT_FLAG_ENABLE,
    ZP_COMPONENT_FLAG_DIRTY,
    ZP_COMPONENT_FLAG_SHOULD_DESTROY,

    zpComponentFlags_Count,
    ZP_COMPONENT_FLAG_USER_START = zpComponentFlags_Count,
};

//
//
//

ZP_ABSTRACT_CLASS zpComponent
{
    ZP_NON_COPYABLE( zpComponent );
public:

    zpComponent()
        : m_instanceId( 0 )
        , m_flags( 0 )
    {
    }

    virtual ~zpComponent()
    {
    }

    zp_hash64 getInstanceId() const
    {
        return m_instanceId;
    }

    void update( zp_float dt, zp_float rt )
    {
        if( !zp_flag32_is_set( m_flags, ZP_COMPONENT_FLAG_STARTED ) )
        {
            onStart();

            m_flags = zp_flag32_mark( m_flags, ZP_COMPONENT_FLAG_STARTED );
        }

        onUpdate( dt, rt );
    }

    void lateUpdate( zp_float dt, zp_float rt )
    {
        onLateUpdate( dt, rt );
    }

    void fixedUpdate( zp_float ft, zp_float rt )
    {
        onFixedUpdate( ft, rt );
    }

    zp_bool isEnabled() const
    {
        return zp_flag32_is_set( m_flags, ZP_COMPONENT_FLAG_ENABLE );
    }

    void setEnabled( zp_bool enabled )
    {
        const zp_bool wasEnabled = zp_flag32_is_set( m_flags, ZP_COMPONENT_FLAG_ENABLE );
        m_flags = zp_flag32_set( m_flags, ZP_COMPONENT_FLAG_ENABLE, enabled );
        const zp_bool nowEnabled = zp_flag32_is_set( m_flags, ZP_COMPONENT_FLAG_ENABLE );

        if( !wasEnabled && nowEnabled )
        {
            onEnabled();
        }
        else if( wasEnabled && !nowEnabled )
        {
            onDisabled();
        }
    }

    void create()
    {
        onCreated();

        m_flags = zp_flag32_mark( m_flags, ZP_COMPONENT_FLAG_CREATED );
    }

    void destroy()
    {
        m_flags = zp_flag32_clear( m_flags, ZP_COMPONENT_FLAG_ENABLE );

        onDisabled();

        onDestroyed();

        m_flags = zp_flag32_mark( m_flags, ZP_COMPONENT_FLAG_SHOULD_DESTROY );
    }

    void setParentObject( const zpObjectHandle& parentObject )
    {
        m_parentObject = parentObject;
    }

    const zpObjectHandle& getParentObject() const
    {
        return m_parentObject;
    }

    zpObjectHandle getParentObject()
    {
        return m_parentObject;
    }

    void setInstanceId( zp_hash64 instanceId )
    {
        m_instanceId = instanceId;
    }

    ZP_INLINE zp_bool isFlagSet( zp_uint index ) const
    {
        return zp_flag32_is_set( m_flags, index );
    }

protected:
    virtual void onStart() {}

    virtual void onEnabled() {}
    virtual void onDisabled() {}

    virtual void onCreated() {}
    virtual void onDestroyed() {}

    virtual void onUpdate( zp_float dt, zp_float rt ) {}
    virtual void onLateUpdate( zp_float dt, zp_float rt ) {}
    virtual void onFixedUpdate( zp_float ft, zp_float rt ) {}

    ZP_INLINE void markFlag( zp_uint index )
    {
        m_flags = zp_flag32_mark( m_flags, index );
    }

    ZP_INLINE void clearFlag( zp_uint index )
    {
        m_flags = zp_flag32_clear( m_flags, index );
    }

    ZP_INLINE void toggleFlag( zp_uint index )
    {
        m_flags = zp_flag32_toggle( m_flags, index );
    }

    ZP_INLINE void setFlag( zp_uint index, zp_bool set )
    {
        m_flags = zp_flag32_set( m_flags, index, set );
    }

private:

    zpObjectHandle m_parentObject;

    zp_hash64 m_instanceId;
    zp_flag32 m_flags;
};

//
//
//

template< typename Component >
ZP_ABSTRACT_CLASS zpComponentManager : public zpHandleManager< Component >
{
    ZP_NON_COPYABLE( zpComponentManager );
public:
    //typedef zpHandleManager< Component >::handle_reference handle_reference;
    //typedef zpHandleManager< Component >::handle_const_reference handle_const_reference;
    //
    //typedef zpHandleManager< Component >::instance_pointer instance_pointer;
    //typedef zpHandleManager< Component >::instance_const_pointer instance_const_pointer;

    zpComponentManager()
    {
    }

    virtual ~zpComponentManager()
    {
    }

    void update( zp_float dt, zp_float rt )
    {
        ZP_PROFILER_BLOCK();

        onPreUpdate( dt, rt );

        zpVector< instance_pointer >::iterator b = m_instances.begin();
        zpVector< instance_pointer >::iterator e = m_instances.end();

        for( ; b != e; ++b )
        {
            zpVector< instance_pointer >::value_type p = *b;
            if( !p->component.isFlagSet( ZP_COMPONENT_FLAG_CREATED ) )
            {
                p->component.create();
            }

            if( p->component.isEnabled() )
            {
                p->component.update( dt, rt );
            }
        }

        onPostUpdate( dt, rt );
    }

    void fixedUpdate( zp_float ft, zp_float rt )
    {
        ZP_PROFILER_BLOCK();

        onPreFixedUpdate( ft, rt );

        zpVector< instance_pointer >::iterator b = m_instances.begin();
        zpVector< instance_pointer >::iterator e = m_instances.end();

        for( ; b != e; ++b )
        {
            zpVector< instance_pointer >::value_type p = *b;
            if( p->component.isEnabled() )
            {
                p->component.fixedUpdate( ft, rt );
            }
        }

        onPostFixedUpdate( ft, rt );
    }

    void lateUpdate( zp_float dt, zp_float rt )
    {
        ZP_PROFILER_BLOCK();

        onPreLateUpdate( dt, rt );

        zpVector< instance_pointer >::iterator b = m_instances.begin();
        zpVector< instance_pointer >::iterator e = m_instances.end();

        for( ; b != e; ++b )
        {
            zpVector< instance_pointer >::value_type p = *b;
            if( p->component.isEnabled() )
            {
                p->component.lateUpdate( dt, rt );
            }
        }

        onPostLateUpdate( dt, rt );
    }


protected:
    virtual void onPreUpdate( zp_float dt, zp_float rt ) {}
    virtual void onPostUpdate( zp_float dt, zp_float rt ) {}

    virtual void onPreLateUpdate( zp_float dt, zp_float rt ) {}
    virtual void onPostLateUpdate( zp_float dt, zp_float rt ) {}

    virtual void onPreFixedUpdate( zp_float ft, zp_float rt ) {}
    virtual void onPostFixedUpdate( zp_float ft, zp_float rt ) {}
};

#endif // !ZP_COMPONENT_H
