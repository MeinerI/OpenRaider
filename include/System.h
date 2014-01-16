/*!
 * \file include/System.h
 * \brief Mostly defines the interface of System implementations.
 *
 * Currently only SDL is used, but there was a GLUT implementation.
 *
 * \author Mongoose
 * \author xythobuz
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Map.h>
#include <Vector.h>

//! \todo Replace with unicode compatible key codes
#define SYS_MOUSE_LEFT    6000
#define SYS_MOUSE_RIGHT   6001
#define SYS_MOUSE_MIDDLE  6002
#define SYS_KEY_ESC       27
#define SYS_KEY_ENTER     13
#define SYS_KEY_UP        5000
#define SYS_KEY_DOWN      5001
#define SYS_KEY_RIGHT     5002
#define SYS_KEY_LEFT      5003
#define SYS_KEY_PAGEDOWN  5004
#define SYS_KEY_PAGEUP    5005
#define SYS_KEY_F1        1000
#define SYS_KEY_F2        1001
#define SYS_KEY_F3        1002
#define SYS_KEY_F4        1003
#define SYS_KEY_F5        1004
#define SYS_KEY_F6        1005
#define SYS_KEY_F7        1006
#define SYS_KEY_F8        1007
#define SYS_KEY_F9        1008
#define SYS_KEY_F10       1009
#define SYS_KEY_F11       1010
#define SYS_KEY_F12       1011

typedef enum {
    SYS_MOD_KEY_LSHIFT = 1,
    SYS_MOD_KEY_RSHIFT = 2,
    SYS_MOD_KEY_LCTRL  = 4,
    SYS_MOD_KEY_RCTRL  = 8,
    SYS_MOD_KEY_LALT   = 16,
    SYS_MOD_KEY_RALT   = 32,
    SYS_MOD_KEY_LMETA  = 64,
    SYS_MOD_KEY_RMETA  = 128,
} sdl_sys_mod_key_t;

/*!
 * \brief Basic Interface for System implementations (SDLSystem)
 */
class System
{
public:

    ////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////

    /*!
     * \brief Constructs an object of System
     */
    System();

    /*!
     * \brief Deconstructs an object of System
     */
    virtual ~System();


    ////////////////////////////////////////////////////////////
    // Public Accessors
    ////////////////////////////////////////////////////////////

    /*!
     * \brief Generates a buufered string for the printf call
     * \param string Format string like for printf
     * \returns string in a buffer
     */
    static char *bufferString(const char *string, ...);

    /*!
     * \brief Expansion of unix home enviroment char.
     * Also makes sure string ends in "end" char.
     * \param path path string
     * \param end end character. 0 appends no additional char
     * \returns allocated string of path with expansions
     */
    static char *fullPath(const char *path, char end);

    /*!
     * \brief Only returns last part of a path string.
     * \param filename Path to a file
     * \returns Name of the file in filename, without path in front
     */
    static char *getFileFromFullPath(char *filename);

    /*!
     * \brief Gets the game tick
     * \returns number of milliseconds since start of program
     */
    virtual unsigned int getTicks();

    /*!
     * \brief Downloads something into passed buffer
     * \todo Not yet implemented!
     * \param urlString URL of thing to download
     * \param buffer array of strings as target
     * \param size size of  buffer
     * \returns < 0 on error, 0 on success
     */
    static int downloadToBuffer(char *urlString,
                                         unsigned char **buffer, unsigned int *size);

    /*!
     * \brief Downloads something into a disk file.
     * Supports HTTP and FTP.
     * \param urlString URL of thing to download
     * \param filename file that should be created/filled
     * \returns < 0 on error, 0 on success. -1000 if libferit not linked
     */
    static int downloadToFile(char *urlString, char *filename);

    /*!
     * \brief Created a directory
     * \param path Directory to create
     * \returns -1 on error
     */
    static int createDir(char *path);


    ////////////////////////////////////////////////////////////
    // Public Mutators
    ////////////////////////////////////////////////////////////

    /*!
     * \brief Created a new Command Mode.
     * \param command valid command mode for the resource file, eg "[Engine.OpenGL.Driver]"
     * \returns id given to mode
     */
    virtual unsigned int addCommandMode(const char *command);

    /*!
     * \brief Binds a key to a command
     * \param cmd valid command string for event
     * \param key valid keyboard code
     * \param event valid game event id
     */
    virtual void bindKeyCommand(const char *cmd, unsigned int key, int event);

    /*!
     * \brief Executes a command string
     * \param cmd valid command string, cmd sets its var
     */
    virtual void command(const char *cmd);


    virtual void gameFrame() = 0;


    virtual void handleMouseMotionEvent(float x, float y) = 0;

    /*!
     * \brief Receives the event bound to the command from the key press
     * \param key key pressed
     */
    virtual void handleBoundKeyPressEvent(unsigned int key) = 0;

    /*!
     * \brief Receives the event bound to the command from the key release
     * \param key key released
     */
    virtual void handleBoundKeyReleaseEvent(unsigned int key) = 0;

    /*!
     * \brief Executes valid command based on keyword
     * \param command valid keyword, optionally followed by space separated arguments
     * \param mode current type or resource mode
     */
    virtual void handleCommand(char *command, unsigned int mode) = 0;

    /*!
     * \brief Receives key code from text input in console mode
     * \param key is a valid keyboard code
     * \param mod modifier key
     */
    virtual void handleConsoleKeyPressEvent(unsigned int key,
                                                         unsigned int mod) = 0;


    virtual void handleKeyPressEvent(unsigned int key, unsigned int mod) = 0;


    virtual void handleKeyReleaseEvent(unsigned int key, unsigned int mod) = 0;


    virtual void initGL();


    virtual void initVideo(unsigned int width, unsigned int height,
                                  bool fullscreen) = 0;

    /*!
     * \brief Init the resource vars
     * \param filename resource file
     * \returns < 0 on error
     */
    virtual int loadResourceFile(const char *filename);


    static void resetTicks();


    virtual void resizeGL(unsigned int width, unsigned int height);


    virtual void runGame() = 0;

    /*!
     * \brief Turns console key events on/off
     * Mostly for allowing text entry vs key impulse commands
     * \param on new state
     */
    void setConsoleMode(bool on);


    void setDriverGL(const char *driver);


    void setFastCardPerformance(bool isFast);


    virtual void shutdown(int code) = 0;


    virtual void swapBuffersGL() = 0;


    virtual void toggleFullscreen() = 0;

protected:

    unsigned int m_width; //!< Width of the viewport
    unsigned int m_height; //!< Height of the viewport
    bool m_fastCard; //!< Assume expensive calls are fine if true
    char *m_driver; //!< String for dynamic use of GL library
    float m_clipNear; //!< Clip near distance
    float m_clipFar; //!< Clip far distance
    float m_fovY; //!< Field of vision
    Map<unsigned int, int> mKeyEvents; //!< Single key press event mappings
    bool mConsoleMode; //!< Using text (console) event handler?
    Vector<const char *> mCmdModes; //!< Dynamic resource command collection
    unsigned int mCommandMode; //!< Current resource command mode
    unsigned int mConsoleKey; //!< Console toggle event now handled lower

private:

    ////////////////////////////////////////////////////////////
    // Private Accessors
    ////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////
    // Private Mutators
    ////////////////////////////////////////////////////////////
};


//! \todo Could make these static methods later, depends on API evolution

    /*!
     * \brief Checks if Command matches Symbol.
     * Returns the rest of the argument list back in command buffer, if any
     * \param symbol command string
     * \param command with arguments
     * \returns true if command matches symbol
     */
    bool rc_command(const char *symbol, char *command);

    /*!
     * \brief Interpret a string as a bool
     * \param buffer "true" or "false"
     * \param val is set to boolean interpretation of buffer
     * \returns -1 for null string, -2 if string is not "true" or "false"
     */
    int rc_get_bool(char *buffer, bool *val);

    /*!
     * \brief Sets timer state and returns number of ticks
     * \param state 0 - reset, 1 - get number of ticks
     * \returns number of ticks
     */
    unsigned int system_timer(int state);

#endif