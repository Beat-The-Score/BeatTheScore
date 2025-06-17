#include "input.h"

Input::Input() {

}

QString Input::getName()
{
    return "Demo";
}

void Input::listen()
{
    listening = true;
}

void Input::stop()
{
    listening = false;
}

bool Input::isListening()
{
    return listening;
}

InputType Input::getType()
{
    return DEMO;
}
