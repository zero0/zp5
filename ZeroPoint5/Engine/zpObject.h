#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

class zpObject
{
public:
    zpObject();
    ~zpObject();

    zp_hash64 getInstanceId() const;

    const zpString& getName() const;
    void setName( const zp_char* name );

    zp_ulong getLayers() const;
    void setLayers( zp_ulong layers );
    void markOnLayer( zp_int layerIndex, zp_bool onLayer );
    zp_bool isOnLayer( zp_int layerIndex ) const;

    zp_ulong getTags() const;
    void setTags( zp_ulong tags );
    void markOnTag( zp_int tagIndex, zp_bool tagged );
    zp_bool hasTag( zp_int tagIndex ) const;

    const zpAllComponents* getAllComponents() const;
    zpAllComponents* getAllComponents();

    void setActive( zp_bool isActive );
    zp_bool isActive() const;

    void destroy();

private:
    enum
    {
        ZP_OBJECT_FLAG_ACTIVE,
        ZP_OBJECT_FLAG_SHOULD_DESTROY,
    };

    void setInstanceId( zp_hash64 instanceId );
    zp_bool shouldDestroy() const;

    zp_hash64 m_instanceId;

    zp_ulong m_layers;
    zp_ulong m_tags;
    zp_ulong m_flags;

    zpString m_name;

    zpAllComponents m_allComponents;

    friend class zpObjectManager;
};

struct zpObjectInstance
{
    zpObject object;
    zp_size_t refCount;
};

#endif // !ZP_OBJECT_H
