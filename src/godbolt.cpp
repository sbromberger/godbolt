#include "godbolt.hpp"
#include "cxxopts.hpp"
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>

const std::string BASE_URL = "https://godbolt.org/";
const std::string DEFAULT_COMPILER = "cclang1810";

std::string read_file(const std::string &fn) {
  std::ifstream f(fn); // taking file as inputstream
  std::ostringstream ss{};
  if (f) {
    ss << f.rdbuf(); // reading data
  }
  return ss.str();
}

int main(int argc, char const *argv[]) {
  std::string fn;
  std::string compiler = DEFAULT_COMPILER;
  cxxopts::Options options(
      argv[0], "Post file(s) to compiler explorer from the command line");

  options.add_options()(
      "c", "The compiler type",
      cxxopts::value<std::string>()->default_value(DEFAULT_COMPILER))(
      "filenames", "The filename(s) to process",
      cxxopts::value<std::vector<std::string>>())("h,help", "Print usage");
  options.parse_positional({"filenames"});
  auto result = options.parse(argc, argv);
  if (result.count("help") > 0) {
    std::cout << options.help() << "\n";
    exit(0);
  }

  godbolt::GBSessions g{};
  int fn_ind = 1;
  for (const auto &fn : result["filenames"].as<std::vector<std::string>>()) {
    std::cout << "Processing " << fn << " with compiler " << compiler << "\n";

    std::string contents = read_file(fn);
    // std::cout << "contents = " << contents << "\n";
    g.emplace_back(fn_ind, contents);
    fn_ind++;
  }
  std::cout << "Calling post_it, g.size = " << g.size() << "\n";
  std::cout << godbolt::post_it(g);
  return 0;
}
