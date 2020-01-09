#pragma once

#include <vector>

#include "Storage.h"
#include "EventManager.h"
#include "EventHandler.h"
#include "Window.h"

namespace activity
{

class Activity;

class LogicBlock
{
public:
    virtual int init(Activity *);
    virtual int update(Activity *) = 0;
};

class Activity
{
public:
    Activity();
    virtual void init();
    virtual void terminate();

    virtual basic::Storage mainLoop();
    void stop();

    void addLogicBlock(activity::LogicBlock *cond);
    void clearConditions();

    virtual void processInput() = 0;
    virtual void updateConditions();
    virtual void generateOutput() = 0;

protected:
    bool onRun;
    std::vector<activity::LogicBlock *> conditions;
    basic::Storage storage;
};

class QuitHandler : public control::EventHandler
{
public:
    QuitHandler() = delete;
    QuitHandler(Activity *windowActivity);

    void handleEvent(const SDL_Event &event) override;

private:
    Activity *activity;
};

class WindowActivity : public Activity
{
public:
    control::EventManager *eventManager;

    WindowActivity() = delete;
    WindowActivity(os::Window *window);

    virtual void init() override;

    virtual void terminate() override;

    virtual void processInput() override;

    virtual void generateOutput() override;

    WindowActivity(const WindowActivity &other) = delete;
    WindowActivity &operator=(const WindowActivity &other) = delete;

    virtual ~WindowActivity();

protected:
    os::Window *window;
    QuitHandler *quitHandler;
};
} // namespace activity
