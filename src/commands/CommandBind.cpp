/*!
 * \file src/commands/CommandBind.cpp
 * \brief Bind command
 *
 * \author xythobuz
 */

#include "global.h"
#include "Console.h"
#include "OpenRaider.h"
#include "commands/CommandBind.h"

std::string CommandBind::name() {
    return "bind";
}

std::string CommandBind::brief() {
    return "bind a keyboard/mouse action";
}

void CommandBind::printHelp() {
    getConsole() << "bind-Command Usage:" << Console::endl;
    getConsole() << "  bind ACTION KEY" << Console::endl;
    getConsole() << "Available Actions:" << Console::endl;
    getConsole() << "  menu" << Console::endl;
    getConsole() << "  console" << Console::endl;
    getConsole() << "  forward" << Console::endl;
    getConsole() << "  backward" << Console::endl;
    getConsole() << "  left" << Console::endl;
    getConsole() << "  right" << Console::endl;
    getConsole() << "  jump" << Console::endl;
    getConsole() << "  crouch" << Console::endl;
    getConsole() << "  use" << Console::endl;
    getConsole() << "  holster" << Console::endl;
    getConsole() << "  walk" << Console::endl;
    getConsole() << "Key-Format:" << Console::endl;
    getConsole() << "  'a' or '1'    for character/number keys" << Console::endl;
    getConsole() << "  \"leftctrl\"  for symbols and special keys" << Console::endl;
}

int CommandBind::execute(std::istream& args) {
    std::string a, b;
    if (!(args >> a >> b)) {
        getConsole() << "Invalid use of bind-command" << Console::endl;
        return -1;
    } else {
        ActionEvents e = stringToActionEvent(a);
        if (e == ActionEventCount) {
            getConsole() << "bind-Error: Unknown action (" << a << ")" << Console::endl;
            return -2;
        }

        KeyboardButton c = stringToKeyboardButton(b);
        if (c == unknownKey) {
            getConsole() << "bind-Error: Unknown key (" << b << ")" << Console::endl;
            return -3;
        }

        getOpenRaider().keyBindings[e] = c;
        return 0;
    }
}

ActionEvents CommandBind::stringToActionEvent(std::string action) {
    if (action == "menu") {
        return menuAction;
    } else if (action == "console") {
        return consoleAction;
    } else if (action == "forward") {
        return forwardAction;
    } else if (action == "backward") {
        return backwardAction;
    } else if (action == "left") {
        return leftAction;
    } else if (action == "right") {
        return rightAction;
    } else if (action == "jump") {
        return jumpAction;
    } else if (action == "crouch") {
        return crouchAction;
    } else if (action == "use") {
        return useAction;
    } else if (action == "holster") {
        return holsterAction;
    } else if (action == "walk") {
        return walkAction;
    } else {
        return ActionEventCount;
    }
}

KeyboardButton CommandBind::stringToKeyboardButton(std::string key) {
    if ((key.length() == 3) && (key[0] == '\'') && (key[2] == '\'')) {
        // Literal character like w, a, s, d, 0, 1...
        char c = key[1];
        if (((c >= '0') && (c <= '9'))
            || ((c >= 'a') && (c <= 'z')))
            return (KeyboardButton)c;
    } else if ((key.length() >= 3) && (key[0] == '\"') && (key[key.length() - 1] == '\"')) {
        // Special characters like tilde, esc, quote...
        key.erase(key.length() - 1); // Delete " at end
        key.erase(0, 1); // Delete " at beginning
        if (key == "quote") {
            return quoteKey;
        } else if (key == "backslash") {
            return backslashKey;
        } else if (key == "backspace") {
            return backspaceKey;
        } else if (key == "capslock") {
            return capslockKey;
        } else if (key == "comma") {
            return commaKey;
        } else if (key == "del") {
            return delKey;
        } else if (key == "up") {
            return upKey;
        } else if (key == "down") {
            return downKey;
        } else if (key == "left") {
            return leftKey;
        } else if (key == "right") {
            return rightKey;
        } else if (key == "end") {
            return endKey;
        } else if (key == "equals") {
            return equalsKey;
        } else if (key == "escape") {
            return escapeKey;
        } else if (key == "f1") {
            return f1Key;
        } else if (key == "f2") {
            return f2Key;
        } else if (key == "f3") {
            return f3Key;
        } else if (key == "f4") {
            return f4Key;
        } else if (key == "f5") {
            return f5Key;
        } else if (key == "f6") {
            return f6Key;
        } else if (key == "f7") {
            return f7Key;
        } else if (key == "f8") {
            return f8Key;
        } else if (key == "f9") {
            return f9Key;
        } else if (key == "f10") {
            return f10Key;
        } else if (key == "f11") {
            return f11Key;
        } else if (key == "f12") {
            return f12Key;
        } else if (key == "backquote") {
            return backquoteKey;
        } else if (key == "home") {
            return homeKey;
        } else if (key == "insert") {
            return insertKey;
        } else if (key == "leftalt") {
            return leftaltKey;
        } else if (key == "leftctrl") {
            return leftctrlKey;
        } else if (key == "leftbracket") {
            return leftbracketKey;
        } else if (key == "leftgui") {
            return leftguiKey;
        } else if (key == "leftshift") {
            return leftshiftKey;
        } else if (key == "minus") {
            return minusKey;
        } else if (key == "numlock") {
            return numlockKey;
        } else if (key == "pagedown") {
            return pagedownKey;
        } else if (key == "pageup") {
            return pageupKey;
        } else if (key == "pause") {
            return pauseKey;
        } else if (key == "dot") {
            return dotKey;
        } else if (key == "rightalt") {
            return rightaltKey;
        } else if (key == "rightctrl") {
            return rightctrlKey;
        } else if (key == "enter") {
            return enterKey;
        } else if (key == "rightgui") {
            return rightguiKey;
        } else if (key == "rightbracket") {
            return rightbracketKey;
        } else if (key == "rightshift") {
            return rightshiftKey;
        } else if (key == "scrolllock") {
            return scrolllockKey;
        } else if (key == "semicolon") {
            return semicolonKey;
        } else if (key == "slash") {
            return slashKey;
        } else if (key == "space") {
            return spaceKey;
        } else if (key == "tab") {
            return tabKey;
        } else if (key == "leftmouse") {
            return leftmouseKey;
        } else if (key == "middlemouse") {
            return middlemouseKey;
        } else if (key == "rightmouse") {
            return rightmouseKey;
        } else if (key == "fourthmouse") {
            return fourthmouseKey;
        } else if (key == "fifthmouse") {
            return fifthmouseKey;
        }
    }

    return unknownKey;
}

