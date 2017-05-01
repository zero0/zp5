#pragma once
#ifndef ZP_DEBUG_H
#define ZP_DEBUG_H

enum
{
    ZP_DEBUG_DISPLAY_PROFILER_STATS = 1 << 0,
    ZP_DEBUG_DISPLAY_OBJECT_STATS =   1 << 1,
};

enum zpDebugGUIWidgetType : zp_uint
{
    ZP_WIDGET_TYPE_WINDOW,
    ZP_WIDGET_TYPE_CONTAINER,
    ZP_WIDGET_TYPE_BOX,
    ZP_WIDGET_TYPE_LABEL,
    ZP_WIDGET_TYPE_BUTTON,

    zpDebugGUIWidgetType_Count,
    zpDebugGUIWidgetType_Force32 = ZP_FORECE_32BIT
};

struct zpDebugGUIWidgetStyle
{
    zpColor32i backgroundColor;
    zpColor32i foregroundColor;
    zpColor32i contentColor;
    zp_uint fontSize;
};

struct zpDebugGUIWidgetLayout
{
    zpRecti rect;
    zpVector2i offset;
    zpVector2i padding;
    zpVector2i margin;
    zp_bool expandWidth;
    zp_bool expandHeight;
};

struct zpDebugGUIWidget
{
    zp_size_t frameUpdated;
    zp_size_t parentWidget;
    zpDebugGUIWidgetLayout layout;

    zpDebugGUIWidgetType type;
    zpDebugGUIWidgetStyle style;

    zpString text;
    zpVector< zp_size_t> children;
};

class zpDebugGUI
{
public:
    zpDebugGUI();
    ~zpDebugGUI();

    void setup( zpInput* input);
    void teardown();

    void startGUI();
    void update( zp_float dt, zp_float rt );
    void render( zpRenderingContext* ctx );
    void endGUI();

    void beginWindow( zpRecti& area );
    void endWindow();

    void beginVertical();
    void endVertical();

    void beginHorizontal();
    void endHorizontal();

    void beginScrollArea( zpVector2i& scroll );
    void endScrollArea();

    void label( const zp_char* label );
    zp_bool button( const zp_char* label );
    void box();

private:
    const zp_size_t npos = static_cast<zp_size_t>( -1 );

    void updateWidget( zp_size_t widgetIndex );

    zpInput* m_input;

    zp_ulong m_flags;
    zp_size_t m_frame;
    zpVector2i m_screenSize;

    zpDebugGUIWidgetStyle m_style;

    zpFontHandle m_debugFont;
    zpMaterialHandle m_debugMaterial;

    zpVector< zpDebugGUIWidget > m_widgets;
    zpVector< zp_size_t > m_widgetStack;
};

#endif // !ZP_DEBUG_H
