#ifndef TEXT_ENGINE_HPP
#define TEXT_ENGINE_HPP

#include <string>

// namespace for colorful text
namespace Utils {
	namespace TextEngine {

		const std::string ANSI_RESET = "\033[0m";
		const std::string ANSI_BOLD = "\033[1m";
		const std::string ANSI_UNDERLINE = "\033[4m";
		const std::string ANSI_BLINK = "\033[5m";

		const std::string ANSI_FG_BLACK = "\033[30m";
		const std::string ANSI_FG_RED = "\033[31m";
		const std::string ANSI_FG_GREEN = "\033[32m";
		const std::string ANSI_FG_YELLOW = "\033[33m";
		const std::string ANSI_FG_BLUE = "\033[34m";
		const std::string ANSI_FG_MAGENTA = "\033[35m";
		const std::string ANSI_FG_CYAN = "\033[36m";
		const std::string ANSI_FG_WHITE = "\033[37m";

		const std::string ANSI_BG_BLACK = "\033[40m";
		const std::string ANSI_BG_RED = "\033[41m";
		const std::string ANSI_BG_GREEN = "\033[42m";
		const std::string ANSI_BG_YELLOW = "\033[43m";
		const std::string ANSI_BG_BLUE = "\033[44m";
		const std::string ANSI_BG_MAGENTA = "\033[45m";
		const std::string ANSI_BG_CYAN = "\033[46m";
		const std::string ANSI_BG_WHITE = "\033[47m";

		void	yellow();
		void	red();
		void	green();
		void	blue();
		void	reset();
		void	bold();
		void	underline();
		void	blink();
		void	black();
		void	magenta();
		void	cyan();
		void	orange();
		void	white();
		void	redBackground();
		void	greenBackground();
		void	blueBackground();
		void	yellowBackground();
		void	blackBackground();
		void	magentaBackground();
		void	cyanBackground();
		void	whiteBackground();
		void	resetBackground();
		void	resetForeground();
		void	resetAll();
	}
}

#endif
