#include <stdio.h>
#include <string.h>

#include "icons-in-terminal.h"

#define COLOR_AND_SIZE(x) x, sizeof x - 1

#ifdef NO_TRUE_COLOURS

//
// Use 256 colours
//
// You can use this script to get the corresponding numbers:
// https://github.com/eikenb/terminal-colors
//
// $./terminal-colors -n
//

# define COLOR(n) COLOR_AND_SIZE("\x1b[38;5;"#n"m")
# define ENDCOLOR ("\e[0m")

# define LIGHT_PINK COLOR(219)
# define LIGHT_BLUE COLOR(39)
# define LIGHT_SILVER COLOR(253)
# define LIGHT_GREEN COLOR(154)
# define LIGHT_CYAN COLOR(123)
# define LIGHT_RED COLOR(203)
# define LIGHT_ORANGE COLOR(209)
# define LIGHT_MAROON COLOR(137)
# define LIGHT_YELLOW COLOR(228)
# define LIGHT_PURPLE COLOR(177)
# define DARK_PINK COLOR(200)
# define DARK_BLUE COLOR(27)
# define DARK_SILVER COLOR(240)
# define DARK_GREEN COLOR(76)
# define DARK_CYAN COLOR(44)
# define DARK_RED COLOR(124)
# define DARK_ORANGE COLOR(166)
# define DARK_MAROON COLOR(94)
# define DARK_YELLOW COLOR(220)
# define DARK_PURPLE COLOR(93)
# define PINK COLOR(201)
# define BLUE COLOR(33)
# define SILVER COLOR(245)
# define GREEN COLOR(82)
# define CYAN COLOR(45)
# define RED COLOR(196)
# define ORANGE COLOR(208)
# define MAROON COLOR(130)
# define YELLOW COLOR(226)
# define PURPLE COLOR(165)

#else // !NO_TRUE_COLOURS

//
// Use 256*256*256 colours (16 millions colours)
//
// Example from hexadecimal numbers:
//
// Red: #FF3A20
// 0xFF = 255
// 0x3A = 58
// 0x20 = 32
//
// red: COLOR(255, 58, 32)
//

# define COLOR(r,g,b) COLOR_AND_SIZE("\x1b[38;2;"#r";"#g";"#b"m")
# define ENDCOLOR ("\x1b[0m")

# define LIGHT_PINK COLOR(244, 143, 177)
# define LIGHT_BLUE COLOR(144, 202, 249)
# define LIGHT_SILVER COLOR(189, 189, 189)
# define LIGHT_GREEN COLOR(105, 240, 174)
# define LIGHT_CYAN COLOR(24, 255, 255)
# define LIGHT_RED COLOR(255, 93, 72)
# define LIGHT_ORANGE COLOR(255, 171, 64)
# define LIGHT_MAROON COLOR(161, 136, 127)
# define LIGHT_YELLOW COLOR(255, 255, 141)
# define LIGHT_PURPLE COLOR(206, 147, 216)
# define DARK_PINK COLOR(173, 20, 87)
# define DARK_BLUE COLOR(21, 101, 192)
# define DARK_SILVER COLOR(117, 117, 117)
# define DARK_GREEN COLOR(0, 200, 83)
# define DARK_CYAN COLOR(0, 184, 212)
# define DARK_RED COLOR(209, 48, 27)
# define DARK_ORANGE COLOR(255, 109, 0)
# define DARK_MAROON COLOR(78, 52, 46)
# define DARK_YELLOW COLOR(255, 234, 0)
# define DARK_PURPLE COLOR(106, 27, 154)
# define PINK COLOR(233, 30, 99)
# define BLUE COLOR(33, 150, 243)
# define SILVER COLOR(158, 158, 158)
# define GREEN COLOR(0, 230, 118)
# define CYAN COLOR(0, 229, 255)
# define RED COLOR(255, 58, 32)
# define ORANGE COLOR(255, 145, 0)
# define MAROON COLOR(121, 85, 72)
# define YELLOW COLOR(255, 255, 0)
# define PURPLE COLOR(156, 39, 176)

#endif // NO_TRUE_COLOURS

typedef enum type_match {
  SUFFIX = 0b1,
  FULL = 0b10,
  PREFIX = 0b100
} e_type_match;

typedef struct {
  char *string;
  size_t len;
  e_type_match type;
} t_match;

typedef struct {
  char *icon;
  t_match *matches;
  char* color;
  size_t color_len;
} t_file_matching;

// http://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME

// March prefix
#define P1(x) (t_match) { x, sizeof x - 1, PREFIX }
#define P2(a, x) P1(a), (t_match) { x, sizeof x - 1, PREFIX }
#define P3(a, b, x) P2(a, b), (t_match) { x, sizeof x - 1, PREFIX }
#define P4(a, b, c, x) P3(a, b, c), (t_match) { x, sizeof x - 1, PREFIX }
#define P5(a, b, c, d, x) P4(a, b, c, d), (t_match) { x, sizeof x - 1, PREFIX }
#define P6(a, b, c, d, e, x) P5(a, b, c, d, e), (t_match) { x, sizeof x - 1, PREFIX }
#define P7(a, b, c, d, e, f, x) P6(a, b, c, d, e, f), (t_match) { x, sizeof x - 1, PREFIX }
#define P(...) GET_MACRO(__VA_ARGS__, P7, P6, P5, P4, P3, P2, P1)(__VA_ARGS__)

// Match suffix
#define S1(x) (t_match) { x, sizeof x - 1, SUFFIX }
#define S2(a, x) S1(a), (t_match) { x, sizeof x - 1, SUFFIX }
#define S3(a, b, x) S2(a, b), (t_match) { x, sizeof x - 1, SUFFIX }
#define S4(a, b, c, x) S3(a, b, c), (t_match) { x, sizeof x - 1, SUFFIX }
#define S5(a, b, c, d, x) S4(a, b, c, d), (t_match) { x, sizeof x - 1, SUFFIX }
#define S6(a, b, c, d, e, x) S5(a, b, c, d, e), (t_match) { x, sizeof x - 1, SUFFIX }
#define S7(a, b, c, d, e, f, x) S6(a, b, c, d, e, f), (t_match) { x, sizeof x - 1, SUFFIX }
#define S(...) GET_MACRO(__VA_ARGS__, S7, S6, S5, S4, S3, S2, S1)(__VA_ARGS__)

// Match full filename
#define F1(x) (t_match) { x, sizeof x - 1, FULL }
#define F2(a, x) F1(a), (t_match) { x, sizeof x - 1, FULL }
#define F3(a, b, x) F2(a, b), (t_match) { x, sizeof x - 1, FULL }
#define F4(a, b, c, x) F3(a, b, c), (t_match) { x, sizeof x - 1, FULL }
#define F5(a, b, c, d, x) F4(a, b, c, d), (t_match) { x, sizeof x - 1, FULL }
#define F6(a, b, c, d, e, x) F5(a, b, c, d, e), (t_match) { x, sizeof x - 1, FULL }
#define F7(a, b, c, d, e, f, x) F6(a, b, c, d, e, f), (t_match) { x, sizeof x - 1, FULL }
#define F(...) GET_MACRO(__VA_ARGS__, F7, F6, F5, F4, F3, F2, F1)(__VA_ARGS__)

#define END() (t_match) { 0 }
#define MATCH(...) (t_match[]) { __VA_ARGS__ , END() }

static t_file_matching file_matches[] = {

  // Insert new data here, not at the end of the structure

  { FILE_VIDEO, MATCH( S(".mpeg", ".mpg") ), RED},
  { FILE_VIDEO, MATCH( S(".webm") ), DARK_BLUE},
  { FILE_VIDEO, MATCH( S(".flv") ), RED},
  { FILE_VIDEO, MATCH( S(".mkv") ), PURPLE},
  { FILE_VIDEO, MATCH( S(".mov") ), CYAN},
  { FILE_VIDEO, MATCH( S(".avi") ), BLUE},
  { FILE_VIDEO, MATCH( S(".mp4", ".m4v", ".h264") ), DARK_BLUE},
  { FILE_VIDEO, MATCH( S(".3gpp", ".3gp") ), BLUE},
  { FA_FILE_CODE_O, MATCH( S(".xml") ), BLUE},
  { FILE_WEBPACK, MATCH( P("webpack.") ), BLUE},
  { OCT_CHECKLIST, MATCH( F("TODO") ), YELLOW},
  { FILE_TAG, MATCH( S(".pid") ), ORANGE},
  { FILE_TAG, MATCH( S(".gemtags") ), RED},
  { FILE_TAG, MATCH( S(".CTAGS", ".TAGS", "CTAGS", "TAGS") ), BLUE},
  { FILE_TAG, MATCH( S(".ctags", ".tags", "ctags", "tags") ), BLUE},
  { DEV_SWIFT, MATCH( S(".swift") ), GREEN},
  { FILE_STYLUS, MATCH( S(".styl", ".stylus") ), GREEN},
  { MFIZZ_SCALA, MATCH( S(".sc", ".scala") ), RED},
  { MFIZZ_PYTHON, MATCH( S(".tac") ), DARK_PINK},
  { MFIZZ_PYTHON, MATCH( S(".pyw") ), MAROON},
  { MFIZZ_PYTHON, MATCH( S(".pyi") ), BLUE},
  { MFIZZ_PYTHON, MATCH( S(".py3") ), DARK_BLUE},
  { MFIZZ_PYTHON, MATCH( S(".pyt") ), DARK_GREEN},
  { MFIZZ_PYTHON, MATCH( S(".pip") ), DARK_PURPLE},
  { MFIZZ_PYTHON, MATCH( S(".gypi") ), DARK_GREEN},
  { MFIZZ_PYTHON, MATCH( S(".gyp") ), GREEN},
  { MFIZZ_PYTHON, MATCH( S(".pep") ), ORANGE},
  { MFIZZ_PYTHON, MATCH( S(".isolate") ), DARK_GREEN},
  { MFIZZ_PYTHON, MATCH( S(".ipy") ), BLUE},
  { MFIZZ_PYTHON, MATCH( S(".py") ), DARK_BLUE},
  { FILE_PHP, MATCH( P("Phakefile") ), DARK_GREEN},
  { FILE_PHP, MATCH( S(".php") ), DARK_BLUE},
  { MFIZZ_PERL, MATCH( S(".psgi", ".xs") ), RED},
  { MFIZZ_PERL, MATCH( S(".pm") ), DARK_BLUE},
  { MFIZZ_PERL, MATCH( S(".plx") ), PURPLE},
  { MFIZZ_PERL, MATCH( S(".ph", "pl") ), DARK_PURPLE},
  { MFIZZ_PERL, MATCH( S(".perl", "pl") ), BLUE},
  { FILE_ORG, MATCH( S(".org") ), DARK_GREEN},
  { MFIZZ_NODEJS, MATCH( S(".node", ".node-version") ), DARK_GREEN},
  { MFIZZ_NODEJS, MATCH( S(".njs", ".nvmrc") ), GREEN},
  { OCT_BOOK, MATCH( F("NEWS", "news") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("CHANGELOG", "changelog", "ChangeLog") ), BLUE},
  { OCT_BOOK, MATCH( P("THANKS.", "thanks.") ), DARK_BLUE},
  { OCT_BOOK, MATCH( P("THANKS-", "thanks-") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("THANKS", "thanks") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("MANIFEST", "manifest") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("MAINTAINERS", "maintainers") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("INSTALL") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("HISTORY") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("AUTHORS") ), LIGHT_BLUE},
  { OCT_BOOK, MATCH( F("HACKING", "hacking") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("COPYING", "copying") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("CONTRIBUTORS", "contributors") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("CONTRIBUTING", "contributing") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("CONTRIBUTE", "contribute") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("CHANGES", "changes") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("BUGS", "bugs") ), DARK_BLUE},
  { OCT_BOOK, MATCH( F("NOTICE", "notice") ), DARK_BLUE},
  { OCT_BOOK, MATCH( P("README.", "readme.") ), YELLOW},
  { OCT_BOOK, MATCH( P("README-", "readme-") ), YELLOW},
  { OCT_BOOK, MATCH( F("README", "readme") ), YELLOW},
  { OCT_MARKDOWN, MATCH( S(".md", ".markdown") ), BLUE},
  { OCT_BOOK, MATCH( F("LICENSE", "license") ), YELLOW},
  { FILE_JULIA, MATCH( S(".jl") ), PURPLE},
  { MFIZZ_REACTJS, MATCH( S(".jsx", ".react", ".react.js") ), BLUE},
  { MFIZZ_JAVA_BOLD, MATCH( S(".java") ), PURPLE},
  { FILE_DASHBOARD, MATCH( S(".cpuprofile") ), GREEN},
  { FILE_DASHBOARD, MATCH( S(".slim", ".skim") ), ORANGE},
  { MFIZZ_RUBY, MATCH( S(".watcher") ), DARK_YELLOW},
  { MFIZZ_RUBY, MATCH( F("rails") ), RED},
  { MFIZZ_RUBY, MATCH( S(".rbuild", ".rbw", ".rbx") ), DARK_RED},
  { MFIZZ_RUBY, MATCH( F(".irbrc", ".gemrc", ".pryrc", ".ruby-gemset", ".ruby-version") ), RED},
  { MFIZZ_RUBY, MATCH( F("irbrc", "gemrc", "pryrc", "ruby-gemset", "ruby-version") ), RED},
  { MFIZZ_RUBY, MATCH( S(".pluginspec", ".podspec", ".rabl", ".rake", ".opal") ), RED},
  { MFIZZ_RUBY, MATCH( S(".ruby", ".rb", ".ru", ".erb", ".gemspec", ".god", ".mspec") ), RED},
  { FILE_HAML, MATCH( S(".haml.deface") ), RED},
  { FILE_HAML, MATCH( S(".hamlc") ), MAROON},
  { FILE_HAML, MATCH( S(".haml") ), YELLOW},
  { MFIZZ_HASKELL, MATCH( S(".lhs") ), DARK_BLUE},
  { MFIZZ_HASKELL, MATCH( S(".c2hs") ), DARK_PURPLE},
  { MFIZZ_HASKELL, MATCH( S(".hsc") ), BLUE},
  { MFIZZ_HASKELL, MATCH( S(".hs") ), PURPLE},
  { FILE_MUSTACHE, MATCH( S(".hbs", ".handlebars", "mustache") ), ORANGE},
  { MFIZZ_GULP, MATCH( F("gulpfile.coffee") ), MAROON},
  { MFIZZ_GULP, MATCH( F("gulpfile.js", "gulpfile.babel.js") ), RED},
  { MFIZZ_GRUNT, MATCH( F("gruntfile.coffee") ), MAROON},
  { MFIZZ_GRUNT, MATCH( F("gruntfile.js") ), YELLOW},
  { FILE_GO, MATCH( S(".go") ), BLUE},
  { MFIZZ_ERLANG, MATCH( F("rebar.config.lock", "rebar.lock") ), RED},
  { MFIZZ_ERLANG, MATCH( F("Emakefile") ), DARK_GREEN},
  { MFIZZ_ERLANG, MATCH( S(".app.src") ), DARK_MAROON},
  { MFIZZ_ERLANG, MATCH( S(".yrl") ), DARK_GREEN},
  { MFIZZ_ERLANG, MATCH( S(".xrl") ), GREEN},
  { MFIZZ_ERLANG, MATCH( S(".hrl") ), MAROON},
  { MFIZZ_ERLANG, MATCH( S(".beam") ), DARK_RED},
  { MFIZZ_ERLANG, MATCH( S(".erl") ), RED},
  { MFIZZ_ELM, MATCH( S(".elm") ), BLUE},
  { MFIZZ_ELIXIR, MATCH( F("mix.ex", "mix.exs", "mix.lock") ), LIGHT_PURPLE},
  { MFIZZ_ELIXIR, MATCH( S(".exs", ".eex") ), PURPLE},
  { MFIZZ_ELIXIR, MATCH( S(".ex") ), DARK_PURPLE},
  { MFIZZ_DOCKER, MATCH( F("docker-sync") ), DARK_ORANGE},
  { MFIZZ_DOCKER, MATCH( F("Dockerfile", "docker-compose"), S(".dockerfile", ".dockerignore") ), DARK_BLUE},
  { DEV_COFFEESCRIPT, MATCH( S(".iced") ), BLUE},
  { DEV_COFFEESCRIPT, MATCH( S(".litcoffee") ), LIGHT_MAROON},
  { DEV_COFFEESCRIPT, MATCH( S(".coffee.erb") ), RED},
  { DEV_COFFEESCRIPT, MATCH( S(".coffee.ecr") ), CYAN},
  { DEV_COFFEESCRIPT, MATCH( S(".cjsx") ), DARK_MAROON},
  { DEV_COFFEESCRIPT, MATCH( S(".coffee") ), MAROON},
  { MFIZZ_CLOJURE, MATCH( S(".hic") ), RED},
  { MFIZZ_CLOJURE, MATCH( S(".cljx") ), RED},
  { MFIZZ_CLOJURE, MATCH( S(".cljc") ), GREEN},
  { MFIZZ_CLOJURE, MATCH( S(".cl2") ), PURPLE},
  { MFIZZ_CLOJURE, MATCH( S(".clj") ), BLUE},
  { MD_VPN_KEY, MATCH( P("id_rsa") ), RED},
  { MD_VPN_KEY, MATCH( S(".crt") ), BLUE},
  { MD_VPN_KEY, MATCH( S(".pem") ), ORANGE},
  { MD_VPN_KEY, MATCH( S(".pub") ), YELLOW},
  { MD_VPN_KEY, MATCH( S(".key") ), BLUE},
  { DEV_BOWER, MATCH( S(".bowerrc", "bower.json", "Bowerfile") ), YELLOW},
  { FILE_BABEL, MATCH( S(".babel", ".babelrc", ".languagebabel") ), YELLOW},
  { FILE_BABEL, MATCH( S(".babelignore") ), DARK_YELLOW},
  { MFIZZ_SASS, MATCH( S(".scss", ".sass") ), PINK},
  { FA_FILE_IMAGE_O, MATCH( S(".gif") ), YELLOW},
  { FA_FILE_IMAGE_O, MATCH( S(".raw") ), DARK_ORANGE},
  { FA_FILE_IMAGE_O, MATCH( S(".bmp") ), RED},
  { FA_FILE_IMAGE_O, MATCH( S(".webp") ), DARK_BLUE},
  { FA_FILE_IMAGE_O, MATCH( S(".ico") ), BLUE},
  { FA_FILE_IMAGE_O, MATCH( S(".jpg") ), GREEN},
  { FA_FILE_IMAGE_O, MATCH( S(".gif") ), YELLOW},
  { FA_FILE_IMAGE_O, MATCH( S(".png") ), ORANGE},
  { FILE_OPENOFFICE, MATCH( S(".odt") ), BLUE},
  { FILE_WORD, MATCH( S(".doc", ".docx") ), BLUE},
  { MFIZZ_DATABASE_ALT2, MATCH( P(".git"),
				F("HEAD", "ORIG_HEAD", "FETCH_HEAD",
				  "packed-refs") ), SILVER},
  { MFIZZ_DATABASE_ALT2, MATCH( S(".qml") ), PINK},
  { MFIZZ_DATABASE_ALT2, MATCH( S(".cson") ), MAROON},
  { MFIZZ_DATABASE_ALT2, MATCH( S(".yaml", ".yml") ), LIGHT_RED},
  { MFIZZ_DATABASE_ALT2, MATCH( S(".json") ), YELLOW},
  { FILE_EMACS, MATCH( S("emacs.d") ), DARK_PURPLE},
  { FILE_EMACS, MATCH( S(".el", ".emacs", ".spacemacs", ".emacs") ), PURPLE},
  { FA_FILE_TEXT_O, MATCH( S(".log", ".journal") ), MAROON},
  { FA_FILE_TEXT_O, MATCH( S(".srt") ), PURPLE},
  { OCT_FILE_BINARY, MATCH( S(".asm", ".S", ".nasm", ".masm") ), LIGHT_BLUE},
  { OCT_FILE_BINARY, MATCH( S(".elf", ".elc") ), LIGHT_PINK},
  { OCT_FILE_BINARY, MATCH( F("a.out") ), DARK_GREEN},
  { OCT_FILE_BINARY, MATCH( S(".bin", ".bsdiff", ".dat",
			      ".pak", ".pdb") ), DARK_ORANGE},
  { OCT_FILE_BINARY, MATCH( S(".objdump", ".d-objdump") ), DARK_BLUE},
  { OCT_FILE_BINARY, MATCH( S(".pyc", ".pyo") ), DARK_PURPLE},
  { FILE_FONT, MATCH( S(".ttf") ), GREEN},
  { FILE_FONT, MATCH( S(".woff2") ), DARK_BLUE},
  { FILE_FONT, MATCH( S(".woff") ), BLUE},
  { FILE_FONT, MATCH( S(".eot") ), LIGHT_GREEN},
  { FILE_FONT, MATCH( S(".ttc") ), DARK_GREEN},
  { FILE_FONT, MATCH( S(".otf") ), DARK_YELLOW},
  { MFIZZ_DEBIAN, MATCH( S(".deb") ), RED},
  { OCT_PACKAGE, MATCH( S(".bundle") ), LIGHT_BLUE},
  { MFIZZ_OSX, MATCH( S(".dmg") ), RED},
  { FA_WINDOWS, MATCH( S(".exe", ".com", ".msi", ".bat",
			 ".cmd", ".reg") ), DARK_PURPLE},
  { MD_MUSIC_NOTE, MATCH( S(".mp3") ), RED},
  { MD_MUSIC_NOTE, MATCH( S(".wma") ), BLUE},
  { MD_MUSIC_NOTE, MATCH( S(".m4a") ), CYAN},
  { MD_MUSIC_NOTE, MATCH( S(".flac") ), DARK_RED},
  { MD_MUSIC_NOTE, MATCH( S(".wav") ), YELLOW},
  { MD_MUSIC_NOTE, MATCH( S(".acc", ".ac3", ".m4p") ), DARK_CYAN},
  { FA_FILE_PDF_O, MATCH( S(".pdf") ), RED},
  { MFIZZ_REDHAT, MATCH( S(".rpm") ), RED},
  { MFIZZ_REDHAT, MATCH( S(".spec") ), DARK_RED},
  { MFIZZ_HTML5, MATCH( S(".html") ), ORANGE},
  { MFIZZ_HTML5, MATCH( S(".html.erb") ), RED},
  { MFIZZ_CSS3, MATCH( S(".less") ), DARK_BLUE},
  { MFIZZ_CSS3, MATCH( S(".css") ), BLUE},
  { MFIZZ_SVG, MATCH( S(".svg") ), DARK_YELLOW},
  { FILE_TEST_JS, MATCH( S(".test.js", ".test.node", ".test._js",
			   ".test.es6", ".test.es", "test-js") ), ORANGE},
  { MFIZZ_NODEJS, MATCH( S(".js", ".node", "._js", ".es6", ".es") ), YELLOW},
  { OCT_TERMINAL, MATCH( S(".tcsh", ".csh") ), YELLOW},
  { OCT_TERMINAL, MATCH( F("depcomp", "libtool", "compile") ), RED},
  { OCT_TERMINAL, MATCH( F("configure", "config.guess", "config.rpath",
			   "config.status", "config.sub", "bootstrap") ), RED},
  { OCT_TERMINAL, MATCH( S(".login", ".profile", ".inputrc") ), RED},
  { OCT_TERMINAL, MATCH( S(".zsh") ), BLUE},
  { OCT_TERMINAL, MATCH( S(".ksh") ), DARK_YELLOW},
  { OCT_TERMINAL, MATCH( S(".bashrc", ".bash_profile") ), DARK_PURPLE},
  { OCT_TERMINAL, MATCH( S(".sh", ".rc", ".bats", ".bash", ".tool",
			    ".install", ".command") ), PURPLE},
  { OCT_TERMINAL, MATCH( S(".fish", ".fishrc") ), GREEN},
  { OCT_FILE_ZIP, MATCH( S(".rar") ), BLUE},
  { FILE_CONFIG, MATCH( S(".conf"), S(".config"), S(".ini"), S(".desktop"),
			S(".cfg"), S(".directory"), S("prefs") ), YELLOW},
  { OCT_FILE_ZIP, MATCH( S(".egg") ), LIGHT_ORANGE},
  { OCT_FILE_ZIP, MATCH( S(".xar") ), DARK_ORANGE},
  { OCT_FILE_ZIP, MATCH( S(".war") ), PURPLE},
  { OCT_FILE_ZIP, MATCH( S(".jar") ), DARK_PINK},
  { OCT_FILE_ZIP, MATCH( S(".epub") ), GREEN},
  { OCT_FILE_ZIP, MATCH( S(".whl") ), DARK_BLUE},
  { OCT_FILE_ZIP, MATCH( S(".gem") ), RED},
  { OCT_FILE_ZIP, MATCH( S(".xpi") ), ORANGE},
  { OCT_FILE_ZIP, MATCH( S(".iso") ), BLUE},
  { OCT_FILE_ZIP, MATCH( S(".nzb") ), LIGHT_MAROON},
  { OCT_FILE_ZIP, MATCH( S(".bz2") ), DARK_CYAN},
  { OCT_FILE_ZIP, MATCH( S(".tar") ), DARK_BLUE},
  { OCT_FILE_ZIP, MATCH( S(".apk") ), RED},
  { OCT_FILE_ZIP, MATCH( S(".7z") ), MAROON},
  { OCT_FILE_ZIP, MATCH( S(".tgz", ".gz") ), DARK_BLUE},
  { OCT_FILE_ZIP, MATCH( S(".zip", ".xz", "z") ), DARK_RED},
  { FILE_CMAKE, MATCH( S(".cmake") ), GREEN},
  { FILE_CMAKE, MATCH( F("CMakeLists.txt") ), RED},
  { MFIZZ_CPLUSPLUS, MATCH( S(".cpp", ".c++", ".cxx", ".cc") ), BLUE},
  { MFIZZ_CPLUSPLUS, MATCH( S(".hh", ".hpp", ".hxx") ), PURPLE},
  { MFIZZ_C, MATCH( S(".c") ), BLUE},
  { MFIZZ_C, MATCH( S(".h") ), PURPLE},
  { FILE_POWERPOINT, MATCH( S(".ppt", ".pps", ".ppsx", ".pptx") ), LIGHT_PINK},
  { OCT_BOOKMARK, MATCH( F("bookmark") ), LIGHT_PINK},
  { FA_FILE_TEXT_O, MATCH( S(".txt", ".text") ), BLUE},
  { OCT_DATABASE, MATCH( S(".cache") ), GREEN},
  { FA_FOLDER, MATCH( F(".", "..") ), DARK_BLUE},
  { OCT_GEAR, MATCH( P(".") ), 0}
  // Insert new data at the beginning of the struture, not here !
};

static const size_t size_file_matches = sizeof file_matches / sizeof file_matches[0];

// Return 0 if filename and the expression correspond
static int match(const char *filename, size_t len, const t_match *match, int is_quoted) {
  const e_type_match type = match->type;
  if (type & PREFIX) {
    return strncmp(filename, match->string, match->len);
  }
  else if (type & SUFFIX) {
    const char *ptr = filename + len - match->len;
    return ptr < filename ? -1 : strncmp(ptr, match->string, match->len);
  }
  else { // if type & FULL
    return len != match->len ? -1 : strncmp(filename, match->string, len);
  }
}

static void write_icon(const char *icon, const char *color, size_t color_len, FILE *stream, int is_colored) {
  int insert_color = !is_colored && color;

  if (insert_color)
    fwrite(color, 1, color_len ,stream);

  fwrite(icon, 1, 4, stream);

  if (insert_color)
    fwrite(ENDCOLOR, 1, sizeof ENDCOLOR - 1,stream);
}

static int _print_icon(const char *filename, const size_t len,
		       FILE *stream, int is_directory, int is_link,
		       int is_quoted, int is_colored) {
  if (is_link) {
    write_icon(OCT_FILE_SYMLINK_FILE, LIGHT_BLUE, stream, is_colored);
    return 0;
  }
  for (int i = 0; i < size_file_matches; i += 1) {
    for (int j = 0; file_matches[i].matches[j].len; j++) {
      if (!match(filename, len, &file_matches[i].matches[j], is_quoted)) {
	write_icon(file_matches[i].icon, file_matches[i].color, file_matches[i].color_len, stream, is_colored);
	return 0;
      }
    }
  }
  if (is_directory)
    write_icon(FA_FOLDER, BLUE, stream, is_colored);
  else
    write_icon(FA_FILE_O, NULL, 0, stream, 0);
  return 0;
}

int print_icon(const char *filename, size_t len, FILE *stream,
	       int is_directory, int is_link, int is_quoted,
	       int is_colored) {
  if (is_quoted) {
    filename += 1;
    len -= 2;
  }
  _print_icon(filename, len, stream, is_directory, is_link, is_quoted, is_colored);
  fputc(' ', stream);
  return 0;
}

int print_arrow_right(FILE *stream) {
  const char arrow[] = " "MD_ARROW_FORWARD"  ";
  fwrite(arrow, 1, sizeof arrow, stream);
  return 0;
}
