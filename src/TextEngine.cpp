#include "../includes/TextEngine.hpp"
#include <iostream>
using namespace std;


namespace Utils {
	// namespace : TextEngine is for text coloring in terminal
	namespace TextEngine {
		void	yellow() { cout << ANSI_FG_YELLOW; }
		void	red() { cout << ANSI_FG_RED; }
		void	green() { cout << ANSI_FG_GREEN; }
		void	blue() { cout << ANSI_FG_BLUE; }
		void	reset() {cout << ANSI_RESET; }
		void	bold() { cout << ANSI_BOLD; }
		void	underline() { cout << ANSI_UNDERLINE;}
		void	blink() { cout << ANSI_BLINK;}
		void	black() { cout << ANSI_FG_BLACK; }
		void	magenta() { cout << ANSI_FG_MAGENTA; }
		void	cyan() { cout << ANSI_FG_CYAN; }
		void	white() { cout << ANSI_FG_WHITE; }
		void	redBackground() { cout << ANSI_BG_RED; }
		void	greenBackground() { cout << ANSI_BG_GREEN; }
		void	blueBackground() { cout << ANSI_BG_BLUE; }
		void	yellowBackground() { cout << ANSI_BG_YELLOW; }
		void	blackBackground() { cout << ANSI_BG_BLACK; }
		void	magentaBackground() { cout << ANSI_BG_MAGENTA; }
		void	cyanBackground() { cout << ANSI_BG_CYAN; }
		void	whiteBackground() { cout << ANSI_BG_WHITE; }
		void	resetBackground() { cout << "\033[49m"; }
		void	resetForeground() { cout << "\033[39m"; }
		void	resetAll() { cout << "\033[0m"; }
		void	orange() { cout << "\033[38;5;208m"; }
	}
}
