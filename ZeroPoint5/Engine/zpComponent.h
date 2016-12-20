#pragma once
#ifndef ZP_COMPONENT_H
#define ZP_COMPONENT_H

ZP_ABSTRACT_CLASS zpComponent
{
    ZP_NON_COPYABLE( zpComponent );
public:
    zpComponent();
    virtual ~zpComponent();

    void update( zp_float dt, zp_float rt );
    void lateUpdate( zp_float dt, zp_float rt );
    void fixedUpdate( zp_float ft, zp_float rt );

    zp_bool isEnabled() const;
    void setEnabled( zp_bool enabled );

    void destroy();

    zpObjectHandle getParentObject() const;

protected:
    virtual void onEnabled() {}
    virtual void onDisabled() {}

    virtual void onCreated() {}
    virtual void onDestroyed() {}

    virtual void onUpdate( zp_float dt, zp_float rt ) {}
    virtual void onLateUpdate( zp_float dt, zp_float rt ) {}
    virtual void onFixedUpdate( zp_float ft, zp_float rt ) {}


private:
    zpObjectHandle m_parentObject;

    zp_bool m_isCreated;
    zp_bool m_isEnabled;
};

#endif // !ZP_COMPONENT_H
