#include <unistd.h>
#include <termios.h>

#include "Game.h"

int main()
{
    // The following code uses termios.h and uninstd.h
    // to replace _kbhit() and _getch()
    //
    // Retrieve the original terminal settings
    struct termios original_settings;
    tcgetattr(STDIN_FILENO, &original_settings);

    // Create a modified copy of the settings
    struct termios modified_settings = original_settings;

    // Disable canonical mode and echoing
    modified_settings.c_lflag &= ~(ICANON | ECHO);

    // Apply the modified settings
    tcsetattr(STDIN_FILENO, TCSANOW, &modified_settings);

    Game game;
    game.Run();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);
    return 0;
}
