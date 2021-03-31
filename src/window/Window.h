#ifndef WINDOW_H_
#define WINDOW_H_

class Window
{
    Window();
    virtual ~Window();

    virtual void createWindowAndContext() = 0;
    virtual void init() = 0;
    virtual void processedEvent() = 0;
};

#endif
