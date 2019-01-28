
#ifndef ANTGUI
#define ANTGUI

typedef struct GUIEntity
{    

} GUIComponent;

typedef struct GUINode
{
    unsigned int border;
    unsigned int padding;
    unsigned int split;
    float splitA;
    float splitB;
} GUINode;

typedef struct GUISystem
{

} GUISystem;

typedef struct GUIClickable
{

} GUIClickable;

GUISystem GUISystemCreate();
GUISystem GUISystemDestroy(const GUISystem * system); 
GUIEntity * GUISystemComponentAdd(const GUISystem * system);




#endif