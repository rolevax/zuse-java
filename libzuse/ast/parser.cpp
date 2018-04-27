#include "parser.h"



namespace zuse
{



void scan_begin(const std::string &filename, bool traceLex);
void scan_end();

std::unique_ptr<AstRoot> parse(const std::string& filename, bool traceLex, bool traceParse)
{
    auto root = std::make_unique<AstRoot>();

	scan_begin(filename, traceLex); 
    yy::BisonParser parser(filename, root.get());
	parser.set_debug_level(traceParse);
	parser.parse(); 
	scan_end();

    return root;
}



} // namespace zuse
