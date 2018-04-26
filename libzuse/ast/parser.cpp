#include "parser.h"



namespace zuse
{



void scan_begin(const std::string &filename, bool traceLex);
void scan_end();

AstRoot *parse(const std::string& filename, bool traceLex, bool traceParse)
{
	AstRoot *root = new AstRoot;

	scan_begin(filename, traceLex); 
	yy::BisonParser parser(filename, root);
	parser.set_debug_level(traceParse);
	parser.parse(); 
	scan_end();

	return root;
}



} // namespace zuse
