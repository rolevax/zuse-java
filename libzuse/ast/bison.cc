// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "bison.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "bison.hh"

// User implementation prologue.

#line 51 "bison.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 38 "bison.yy" // lalr1.cc:413

	#include "lexproto.h"
	#include "parseexception.h"

#line 58 "bison.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 144 "bison.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  BisonParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  BisonParser::BisonParser (const std::string &filename_yyarg, RootAst *result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      filename (filename_yyarg),
      result (result_yyarg)
  {}

  BisonParser::~BisonParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  BisonParser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  BisonParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  BisonParser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  BisonParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  BisonParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  BisonParser::symbol_number_type
  BisonParser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  BisonParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  BisonParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 88: // type_spec
      case 89: // type_name
      case 90: // ptype
      case 94: // class
      case 96: // decl_method
      case 99: // decl_param
      case 101: // stmt
      case 102: // ident
      case 105: // decl_var
      case 107: // dector
      case 108: // dector_name
      case 109: // jump_stmt
      case 110: // while_stmt
      case 111: // do_while_stmt
      case 112: // for_stmt
      case 113: // for_init
      case 116: // catch
      case 118: // expr
      case 119: // expr_lv0
      case 120: // expr_lv1
      case 121: // expr_lv2
      case 122: // expr_lv3
      case 123: // expr_lv4
      case 124: // expr_lv5
      case 125: // expr_lv6
      case 126: // expr_lv7
      case 127: // expr_lv8
      case 128: // expr_lv9
      case 129: // expr_lv10
      case 130: // expr_lv11
      case 131: // expr_lv12
      case 132: // expr_ptype
      case 133: // expr_unary
      case 134: // expr_unary_logic
      case 135: // expr_pp
      case 136: // expr_post
      case 137: // expr_prime
      case 138: // expr_prime_noname
      case 139: // expr_prime_cx
      case 140: // expr_prime_cx_nude
      case 141: // expr_call
      case 142: // expr_field
      case 143: // expr_new
      case 144: // expr_new_plain
      case 145: // dim_exprs
      case 146: // callee
      case 147: // name
      case 149: // special_name
        value.move< Ast* > (that.value);
        break;

      case 93: // class_list
      case 95: // member_list
      case 97: // param_list
      case 98: // param_list_noemp
      case 100: // stmt_list
      case 103: // arg_list
      case 104: // arg_list_noemp
      case 106: // dector_list
      case 114: // try_list
      case 115: // catch_list
      case 117: // if_list
      case 148: // name_list
        value.move< ListAst* > (that.value);
        break;

      case 91: // modifiers
      case 92: // modifier
        value.move< Modifiers > (that.value);
        break;

      case 150: // dims
        value.move< int > (that.value);
        break;

      case 82: // "identifier"
      case 83: // "number"
      case 84: // "string"
      case 85: // "void"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  BisonParser::stack_symbol_type&
  BisonParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 88: // type_spec
      case 89: // type_name
      case 90: // ptype
      case 94: // class
      case 96: // decl_method
      case 99: // decl_param
      case 101: // stmt
      case 102: // ident
      case 105: // decl_var
      case 107: // dector
      case 108: // dector_name
      case 109: // jump_stmt
      case 110: // while_stmt
      case 111: // do_while_stmt
      case 112: // for_stmt
      case 113: // for_init
      case 116: // catch
      case 118: // expr
      case 119: // expr_lv0
      case 120: // expr_lv1
      case 121: // expr_lv2
      case 122: // expr_lv3
      case 123: // expr_lv4
      case 124: // expr_lv5
      case 125: // expr_lv6
      case 126: // expr_lv7
      case 127: // expr_lv8
      case 128: // expr_lv9
      case 129: // expr_lv10
      case 130: // expr_lv11
      case 131: // expr_lv12
      case 132: // expr_ptype
      case 133: // expr_unary
      case 134: // expr_unary_logic
      case 135: // expr_pp
      case 136: // expr_post
      case 137: // expr_prime
      case 138: // expr_prime_noname
      case 139: // expr_prime_cx
      case 140: // expr_prime_cx_nude
      case 141: // expr_call
      case 142: // expr_field
      case 143: // expr_new
      case 144: // expr_new_plain
      case 145: // dim_exprs
      case 146: // callee
      case 147: // name
      case 149: // special_name
        value.copy< Ast* > (that.value);
        break;

      case 93: // class_list
      case 95: // member_list
      case 97: // param_list
      case 98: // param_list_noemp
      case 100: // stmt_list
      case 103: // arg_list
      case 104: // arg_list_noemp
      case 106: // dector_list
      case 114: // try_list
      case 115: // catch_list
      case 117: // if_list
      case 148: // name_list
        value.copy< ListAst* > (that.value);
        break;

      case 91: // modifiers
      case 92: // modifier
        value.copy< Modifiers > (that.value);
        break;

      case 150: // dims
        value.copy< int > (that.value);
        break;

      case 82: // "identifier"
      case 83: // "number"
      case 84: // "string"
      case 85: // "void"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  BisonParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  BisonParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 82: // "identifier"

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 476 "bison.cc" // lalr1.cc:636
        break;

      case 83: // "number"

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 483 "bison.cc" // lalr1.cc:636
        break;

      case 84: // "string"

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 490 "bison.cc" // lalr1.cc:636
        break;

      case 85: // "void"

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 497 "bison.cc" // lalr1.cc:636
        break;

      case 88: // type_spec

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 504 "bison.cc" // lalr1.cc:636
        break;

      case 89: // type_name

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 511 "bison.cc" // lalr1.cc:636
        break;

      case 90: // ptype

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 518 "bison.cc" // lalr1.cc:636
        break;

      case 91: // modifiers

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Modifiers > (); }
#line 525 "bison.cc" // lalr1.cc:636
        break;

      case 92: // modifier

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Modifiers > (); }
#line 532 "bison.cc" // lalr1.cc:636
        break;

      case 93: // class_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 539 "bison.cc" // lalr1.cc:636
        break;

      case 94: // class

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 546 "bison.cc" // lalr1.cc:636
        break;

      case 95: // member_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 553 "bison.cc" // lalr1.cc:636
        break;

      case 96: // decl_method

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 560 "bison.cc" // lalr1.cc:636
        break;

      case 97: // param_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 567 "bison.cc" // lalr1.cc:636
        break;

      case 98: // param_list_noemp

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 574 "bison.cc" // lalr1.cc:636
        break;

      case 99: // decl_param

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 581 "bison.cc" // lalr1.cc:636
        break;

      case 100: // stmt_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 588 "bison.cc" // lalr1.cc:636
        break;

      case 101: // stmt

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 595 "bison.cc" // lalr1.cc:636
        break;

      case 102: // ident

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 602 "bison.cc" // lalr1.cc:636
        break;

      case 103: // arg_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 609 "bison.cc" // lalr1.cc:636
        break;

      case 104: // arg_list_noemp

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 616 "bison.cc" // lalr1.cc:636
        break;

      case 105: // decl_var

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 623 "bison.cc" // lalr1.cc:636
        break;

      case 106: // dector_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 630 "bison.cc" // lalr1.cc:636
        break;

      case 107: // dector

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 637 "bison.cc" // lalr1.cc:636
        break;

      case 108: // dector_name

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 644 "bison.cc" // lalr1.cc:636
        break;

      case 109: // jump_stmt

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 651 "bison.cc" // lalr1.cc:636
        break;

      case 110: // while_stmt

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 658 "bison.cc" // lalr1.cc:636
        break;

      case 111: // do_while_stmt

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 665 "bison.cc" // lalr1.cc:636
        break;

      case 112: // for_stmt

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 672 "bison.cc" // lalr1.cc:636
        break;

      case 113: // for_init

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 679 "bison.cc" // lalr1.cc:636
        break;

      case 114: // try_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 686 "bison.cc" // lalr1.cc:636
        break;

      case 115: // catch_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 693 "bison.cc" // lalr1.cc:636
        break;

      case 116: // catch

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 700 "bison.cc" // lalr1.cc:636
        break;

      case 117: // if_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 707 "bison.cc" // lalr1.cc:636
        break;

      case 118: // expr

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 714 "bison.cc" // lalr1.cc:636
        break;

      case 119: // expr_lv0

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 721 "bison.cc" // lalr1.cc:636
        break;

      case 120: // expr_lv1

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 728 "bison.cc" // lalr1.cc:636
        break;

      case 121: // expr_lv2

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 735 "bison.cc" // lalr1.cc:636
        break;

      case 122: // expr_lv3

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 742 "bison.cc" // lalr1.cc:636
        break;

      case 123: // expr_lv4

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 749 "bison.cc" // lalr1.cc:636
        break;

      case 124: // expr_lv5

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 756 "bison.cc" // lalr1.cc:636
        break;

      case 125: // expr_lv6

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 763 "bison.cc" // lalr1.cc:636
        break;

      case 126: // expr_lv7

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 770 "bison.cc" // lalr1.cc:636
        break;

      case 127: // expr_lv8

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 777 "bison.cc" // lalr1.cc:636
        break;

      case 128: // expr_lv9

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 784 "bison.cc" // lalr1.cc:636
        break;

      case 129: // expr_lv10

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 791 "bison.cc" // lalr1.cc:636
        break;

      case 130: // expr_lv11

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 798 "bison.cc" // lalr1.cc:636
        break;

      case 131: // expr_lv12

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 805 "bison.cc" // lalr1.cc:636
        break;

      case 132: // expr_ptype

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 812 "bison.cc" // lalr1.cc:636
        break;

      case 133: // expr_unary

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 819 "bison.cc" // lalr1.cc:636
        break;

      case 134: // expr_unary_logic

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 826 "bison.cc" // lalr1.cc:636
        break;

      case 135: // expr_pp

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 833 "bison.cc" // lalr1.cc:636
        break;

      case 136: // expr_post

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 840 "bison.cc" // lalr1.cc:636
        break;

      case 137: // expr_prime

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 847 "bison.cc" // lalr1.cc:636
        break;

      case 138: // expr_prime_noname

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 854 "bison.cc" // lalr1.cc:636
        break;

      case 139: // expr_prime_cx

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 861 "bison.cc" // lalr1.cc:636
        break;

      case 140: // expr_prime_cx_nude

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 868 "bison.cc" // lalr1.cc:636
        break;

      case 141: // expr_call

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 875 "bison.cc" // lalr1.cc:636
        break;

      case 142: // expr_field

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 882 "bison.cc" // lalr1.cc:636
        break;

      case 143: // expr_new

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 889 "bison.cc" // lalr1.cc:636
        break;

      case 144: // expr_new_plain

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 896 "bison.cc" // lalr1.cc:636
        break;

      case 145: // dim_exprs

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 903 "bison.cc" // lalr1.cc:636
        break;

      case 146: // callee

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 910 "bison.cc" // lalr1.cc:636
        break;

      case 147: // name

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 917 "bison.cc" // lalr1.cc:636
        break;

      case 148: // name_list

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 924 "bison.cc" // lalr1.cc:636
        break;

      case 149: // special_name

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 931 "bison.cc" // lalr1.cc:636
        break;

      case 150: // dims

#line 206 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< int > (); }
#line 938 "bison.cc" // lalr1.cc:636
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  BisonParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  BisonParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  BisonParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  BisonParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  BisonParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  BisonParser::debug_level_type
  BisonParser::debug_level () const
  {
    return yydebug_;
  }

  void
  BisonParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline BisonParser::state_type
  BisonParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  BisonParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  BisonParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  BisonParser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 28 "bison.yy" // lalr1.cc:741
{
	/* Initialize the initial location. */ 
	/* position.filename is a non-const pointer somehow */
	static std::string s_filename(filename);
	yyla.location.begin.filename = yyla.location.end.filename = &s_filename;
}

#line 1059 "bison.cc" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (filename, result));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 88: // type_spec
      case 89: // type_name
      case 90: // ptype
      case 94: // class
      case 96: // decl_method
      case 99: // decl_param
      case 101: // stmt
      case 102: // ident
      case 105: // decl_var
      case 107: // dector
      case 108: // dector_name
      case 109: // jump_stmt
      case 110: // while_stmt
      case 111: // do_while_stmt
      case 112: // for_stmt
      case 113: // for_init
      case 116: // catch
      case 118: // expr
      case 119: // expr_lv0
      case 120: // expr_lv1
      case 121: // expr_lv2
      case 122: // expr_lv3
      case 123: // expr_lv4
      case 124: // expr_lv5
      case 125: // expr_lv6
      case 126: // expr_lv7
      case 127: // expr_lv8
      case 128: // expr_lv9
      case 129: // expr_lv10
      case 130: // expr_lv11
      case 131: // expr_lv12
      case 132: // expr_ptype
      case 133: // expr_unary
      case 134: // expr_unary_logic
      case 135: // expr_pp
      case 136: // expr_post
      case 137: // expr_prime
      case 138: // expr_prime_noname
      case 139: // expr_prime_cx
      case 140: // expr_prime_cx_nude
      case 141: // expr_call
      case 142: // expr_field
      case 143: // expr_new
      case 144: // expr_new_plain
      case 145: // dim_exprs
      case 146: // callee
      case 147: // name
      case 149: // special_name
        yylhs.value.build< Ast* > ();
        break;

      case 93: // class_list
      case 95: // member_list
      case 97: // param_list
      case 98: // param_list_noemp
      case 100: // stmt_list
      case 103: // arg_list
      case 104: // arg_list_noemp
      case 106: // dector_list
      case 114: // try_list
      case 115: // catch_list
      case 117: // if_list
      case 148: // name_list
        yylhs.value.build< ListAst* > ();
        break;

      case 91: // modifiers
      case 92: // modifier
        yylhs.value.build< Modifiers > ();
        break;

      case 150: // dims
        yylhs.value.build< int > ();
        break;

      case 82: // "identifier"
      case 83: // "number"
      case 84: // "string"
      case 85: // "void"
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 217 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1251 "bison.cc" // lalr1.cc:859
    break;

  case 3:
#line 219 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(yystack_[1].value.as< Ast* > (), yystack_[0].value.as< int > ()); }
#line 1257 "bison.cc" // lalr1.cc:859
    break;

  case 4:
#line 223 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1263 "bison.cc" // lalr1.cc:859
    break;

  case 5:
#line 225 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1269 "bison.cc" // lalr1.cc:859
    break;

  case 6:
#line 229 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new ScalarAst(Ast::Type::IDENT, yystack_[0].value.as< std::string > ()); }
#line 1275 "bison.cc" // lalr1.cc:859
    break;

  case 7:
#line 233 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = yystack_[0].value.as< Modifiers > (); }
#line 1281 "bison.cc" // lalr1.cc:859
    break;

  case 8:
#line 235 "bison.yy" // lalr1.cc:859
    { yystack_[1].value.as< Modifiers > () |= yystack_[0].value.as< Modifiers > (); yylhs.value.as< Modifiers > () = yystack_[1].value.as< Modifiers > (); }
#line 1287 "bison.cc" // lalr1.cc:859
    break;

  case 9:
#line 239 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().abstract = true; }
#line 1293 "bison.cc" // lalr1.cc:859
    break;

  case 10:
#line 241 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().final = true; }
#line 1299 "bison.cc" // lalr1.cc:859
    break;

  case 11:
#line 243 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().access = Modifiers::PUBLIC; }
#line 1305 "bison.cc" // lalr1.cc:859
    break;

  case 12:
#line 245 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().access = Modifiers::PROTECTED; }
#line 1311 "bison.cc" // lalr1.cc:859
    break;

  case 13:
#line 247 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().access = Modifiers::PRIVATE; }
#line 1317 "bison.cc" // lalr1.cc:859
    break;

  case 14:
#line 249 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().statik = true; }
#line 1323 "bison.cc" // lalr1.cc:859
    break;

  case 15:
#line 251 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().transient = true; }
#line 1329 "bison.cc" // lalr1.cc:859
    break;

  case 16:
#line 253 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().voladile = true; }
#line 1335 "bison.cc" // lalr1.cc:859
    break;

  case 17:
#line 255 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().native = true; }
#line 1341 "bison.cc" // lalr1.cc:859
    break;

  case 18:
#line 257 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Modifiers > () = Modifiers(); yylhs.value.as< Modifiers > ().synchronized = true; }
#line 1347 "bison.cc" // lalr1.cc:859
    break;

  case 19:
#line 260 "bison.yy" // lalr1.cc:859
    { /* already newwed as 'result' */ }
#line 1353 "bison.cc" // lalr1.cc:859
    break;

  case 20:
#line 261 "bison.yy" // lalr1.cc:859
    { result->append(yystack_[0].value.as< Ast* > ()); }
#line 1359 "bison.cc" // lalr1.cc:859
    break;

  case 21:
#line 265 "bison.yy" // lalr1.cc:859
    { Ast *h1 = new ScalarAst(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
										 yystack_[3].value.as< Ast* > (), h1, h2, yystack_[1].value.as< ListAst* > ()); }
#line 1368 "bison.cc" // lalr1.cc:859
    break;

  case 22:
#line 270 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
										 yystack_[5].value.as< Ast* > (), yystack_[3].value.as< ListAst* > (), h, yystack_[1].value.as< ListAst* > ()); }
#line 1376 "bison.cc" // lalr1.cc:859
    break;

  case 23:
#line 274 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
										 yystack_[5].value.as< Ast* > (), h, yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); }
#line 1384 "bison.cc" // lalr1.cc:859
    break;

  case 24:
#line 279 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
										 yystack_[7].value.as< Ast* > (), yystack_[5].value.as< ListAst* > (), yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); }
#line 1391 "bison.cc" // lalr1.cc:859
    break;

  case 25:
#line 282 "bison.yy" // lalr1.cc:859
    { Ast *h1 = new ScalarAst(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new ScalarAst(Ast::Type::HIDDEN, "");
				  auto a = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
											 yystack_[3].value.as< Ast* > (), h1, h2, yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[5].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1401 "bison.cc" // lalr1.cc:859
    break;

  case 26:
#line 288 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  auto a = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
											 yystack_[5].value.as< Ast* > (), yystack_[3].value.as< ListAst* > (), h, yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[7].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1410 "bison.cc" // lalr1.cc:859
    break;

  case 27:
#line 293 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  auto a = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
											 yystack_[5].value.as< Ast* > (), h, yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[7].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1419 "bison.cc" // lalr1.cc:859
    break;

  case 28:
#line 299 "bison.yy" // lalr1.cc:859
    { auto a = new FixSizeAst<4>(Ast::Type::DECL_CLASS, 
											 yystack_[7].value.as< Ast* > (), yystack_[5].value.as< ListAst* > (), yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[9].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1427 "bison.cc" // lalr1.cc:859
    break;

  case 29:
#line 303 "bison.yy" // lalr1.cc:859
    { Ast *h1 = new ScalarAst(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
										 yystack_[3].value.as< Ast* > (), h1, h2, yystack_[1].value.as< ListAst* > ()); }
#line 1436 "bison.cc" // lalr1.cc:859
    break;

  case 30:
#line 308 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
										 yystack_[5].value.as< Ast* > (), yystack_[3].value.as< ListAst* > (), h, yystack_[1].value.as< ListAst* > ()); }
#line 1444 "bison.cc" // lalr1.cc:859
    break;

  case 31:
#line 312 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
										 yystack_[5].value.as< Ast* > (), h, yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); }
#line 1452 "bison.cc" // lalr1.cc:859
    break;

  case 32:
#line 317 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
										 yystack_[7].value.as< Ast* > (), yystack_[5].value.as< ListAst* > (), yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); }
#line 1459 "bison.cc" // lalr1.cc:859
    break;

  case 33:
#line 320 "bison.yy" // lalr1.cc:859
    { Ast *h1 = new ScalarAst(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new ScalarAst(Ast::Type::HIDDEN, "");
				  auto a = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
											 yystack_[3].value.as< Ast* > (), h1, h2, yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[5].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1469 "bison.cc" // lalr1.cc:859
    break;

  case 34:
#line 326 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  auto a = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
											 yystack_[5].value.as< Ast* > (), yystack_[3].value.as< ListAst* > (), h, yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[7].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1478 "bison.cc" // lalr1.cc:859
    break;

  case 35:
#line 332 "bison.yy" // lalr1.cc:859
    { Ast *h = new ScalarAst(Ast::Type::HIDDEN, "");
				  auto a = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
											 yystack_[5].value.as< Ast* > (), h, yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[7].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1487 "bison.cc" // lalr1.cc:859
    break;

  case 36:
#line 338 "bison.yy" // lalr1.cc:859
    { auto a = new FixSizeAst<4>(Ast::Type::DECL_INTERFACE, 
											 yystack_[7].value.as< Ast* > (), yystack_[5].value.as< ListAst* > (), yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ());
				  a->setModifiers(yystack_[9].value.as< Modifiers > ()); yylhs.value.as< Ast* > () = a; }
#line 1495 "bison.cc" // lalr1.cc:859
    break;

  case 37:
#line 344 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::MEMBER_LIST); }
#line 1501 "bison.cc" // lalr1.cc:859
    break;

  case 38:
#line 346 "bison.yy" // lalr1.cc:859
    { yystack_[1].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[1].value.as< ListAst* > (); }
#line 1507 "bison.cc" // lalr1.cc:859
    break;

  case 39:
#line 348 "bison.yy" // lalr1.cc:859
    { yystack_[1].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[1].value.as< ListAst* > (); }
#line 1513 "bison.cc" // lalr1.cc:859
    break;

  case 40:
#line 352 "bison.yy" // lalr1.cc:859
    { Ast* hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<5>(Ast::Type::DECL_METHOD, 
										 yystack_[7].value.as< Ast* > (), yystack_[6].value.as< Ast* > (), yystack_[4].value.as< ListAst* > (), hidden, yystack_[1].value.as< ListAst* > ()); }
#line 1521 "bison.cc" // lalr1.cc:859
    break;

  case 41:
#line 357 "bison.yy" // lalr1.cc:859
    { Ast* hidden = new ScalarAst(Ast::Type::HIDDEN, "");
 				  yylhs.value.as< Ast* > () = new FixSizeAst<5>(Ast::Type::DECL_METHOD, 
										 yystack_[7].value.as< Ast* > (), yystack_[6].value.as< Ast* > (), yystack_[4].value.as< ListAst* > (), hidden, yystack_[1].value.as< ListAst* > ()); 
				  yylhs.value.as< Ast* > ()->asFixSize<5>().setModifiers(yystack_[8].value.as< Modifiers > ()); }
#line 1530 "bison.cc" // lalr1.cc:859
    break;

  case 42:
#line 363 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<5>(Ast::Type::DECL_METHOD, 
										 yystack_[9].value.as< Ast* > (), yystack_[8].value.as< Ast* > (), yystack_[6].value.as< ListAst* > (), yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); }
#line 1537 "bison.cc" // lalr1.cc:859
    break;

  case 43:
#line 367 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<5>(Ast::Type::DECL_METHOD, 
										 yystack_[9].value.as< Ast* > (), yystack_[8].value.as< Ast* > (), yystack_[6].value.as< ListAst* > (), yystack_[3].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); 
				  yylhs.value.as< Ast* > ()->asFixSize<5>().setModifiers(yystack_[10].value.as< Modifiers > ()); }
#line 1545 "bison.cc" // lalr1.cc:859
    break;

  case 44:
#line 373 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::DECL_PARAM_LIST); }
#line 1551 "bison.cc" // lalr1.cc:859
    break;

  case 45:
#line 375 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = yystack_[0].value.as< ListAst* > (); }
#line 1557 "bison.cc" // lalr1.cc:859
    break;

  case 46:
#line 378 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::DECL_PARAM_LIST); 
				  yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); }
#line 1564 "bison.cc" // lalr1.cc:859
    break;

  case 47:
#line 381 "bison.yy" // lalr1.cc:859
    { yystack_[2].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[2].value.as< ListAst* > (); }
#line 1570 "bison.cc" // lalr1.cc:859
    break;

  case 48:
#line 385 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL_PARAM, yystack_[1].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1576 "bison.cc" // lalr1.cc:859
    break;

  case 49:
#line 387 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL_PARAM, yystack_[1].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); 
				  yylhs.value.as< Ast* > ()->asFixSize<2>().getModifiers().final = true; }
#line 1583 "bison.cc" // lalr1.cc:859
    break;

  case 50:
#line 392 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::STMT_LIST); }
#line 1589 "bison.cc" // lalr1.cc:859
    break;

  case 51:
#line 394 "bison.yy" // lalr1.cc:859
    { yystack_[1].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[1].value.as< ListAst* > (); }
#line 1595 "bison.cc" // lalr1.cc:859
    break;

  case 52:
#line 398 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[1].value.as< Ast* > (); }
#line 1601 "bison.cc" // lalr1.cc:859
    break;

  case 53:
#line 400 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1607 "bison.cc" // lalr1.cc:859
    break;

  case 54:
#line 402 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1613 "bison.cc" // lalr1.cc:859
    break;

  case 55:
#line 404 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1619 "bison.cc" // lalr1.cc:859
    break;

  case 56:
#line 406 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1625 "bison.cc" // lalr1.cc:859
    break;

  case 57:
#line 408 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1631 "bison.cc" // lalr1.cc:859
    break;

  case 58:
#line 410 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< ListAst* > (); }
#line 1637 "bison.cc" // lalr1.cc:859
    break;

  case 59:
#line 412 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< ListAst* > (); }
#line 1643 "bison.cc" // lalr1.cc:859
    break;

  case 60:
#line 414 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[1].value.as< ListAst* > (); }
#line 1649 "bison.cc" // lalr1.cc:859
    break;

  case 61:
#line 418 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new ScalarAst(Ast::Type::IDENT, yystack_[0].value.as< std::string > ()); }
#line 1655 "bison.cc" // lalr1.cc:859
    break;

  case 62:
#line 422 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::ARG_LIST); }
#line 1661 "bison.cc" // lalr1.cc:859
    break;

  case 63:
#line 424 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = yystack_[0].value.as< ListAst* > (); }
#line 1667 "bison.cc" // lalr1.cc:859
    break;

  case 64:
#line 428 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::ARG_LIST); yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); }
#line 1673 "bison.cc" // lalr1.cc:859
    break;

  case 65:
#line 430 "bison.yy" // lalr1.cc:859
    { yystack_[2].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[2].value.as< ListAst* > (); }
#line 1679 "bison.cc" // lalr1.cc:859
    break;

  case 66:
#line 434 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL_VAR, yystack_[2].value.as< Ast* > (), yystack_[1].value.as< Ast* > ()); }
#line 1685 "bison.cc" // lalr1.cc:859
    break;

  case 67:
#line 436 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL_VAR, yystack_[2].value.as< Ast* > (), yystack_[1].value.as< ListAst* > ()); }
#line 1691 "bison.cc" // lalr1.cc:859
    break;

  case 68:
#line 438 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL_VAR, yystack_[2].value.as< Ast* > (), yystack_[1].value.as< Ast* > ()); 
				  yylhs.value.as< Ast* > ()->asFixSize<2>().setModifiers(yystack_[3].value.as< Modifiers > ()); }
#line 1698 "bison.cc" // lalr1.cc:859
    break;

  case 69:
#line 441 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL_VAR, yystack_[2].value.as< Ast* > (), yystack_[1].value.as< ListAst* > ()); 
				  yylhs.value.as< Ast* > ()->asFixSize<2>().setModifiers(yystack_[3].value.as< Modifiers > ()); }
#line 1705 "bison.cc" // lalr1.cc:859
    break;

  case 70:
#line 446 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::DECTOR_LIST); 
				  yylhs.value.as< ListAst* > ()->append(yystack_[2].value.as< Ast* > ()); yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); }
#line 1712 "bison.cc" // lalr1.cc:859
    break;

  case 71:
#line 449 "bison.yy" // lalr1.cc:859
    { yystack_[2].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[2].value.as< ListAst* > (); }
#line 1718 "bison.cc" // lalr1.cc:859
    break;

  case 72:
#line 453 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1724 "bison.cc" // lalr1.cc:859
    break;

  case 73:
#line 455 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASSIGN, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1730 "bison.cc" // lalr1.cc:859
    break;

  case 74:
#line 459 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1736 "bison.cc" // lalr1.cc:859
    break;

  case 75:
#line 461 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(yystack_[1].value.as< Ast* > (), yystack_[0].value.as< int > ()); }
#line 1742 "bison.cc" // lalr1.cc:859
    break;

  case 76:
#line 465 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::RETURN, yystack_[1].value.as< Ast* > ()); }
#line 1748 "bison.cc" // lalr1.cc:859
    break;

  case 77:
#line 467 "bison.yy" // lalr1.cc:859
    { Ast *hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::RETURN, hidden); }
#line 1755 "bison.cc" // lalr1.cc:859
    break;

  case 78:
#line 470 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::BREAK, yystack_[1].value.as< Ast* > ()); }
#line 1761 "bison.cc" // lalr1.cc:859
    break;

  case 79:
#line 472 "bison.yy" // lalr1.cc:859
    { Ast *hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::BREAK, hidden); }
#line 1768 "bison.cc" // lalr1.cc:859
    break;

  case 80:
#line 475 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::CONTINUE, yystack_[1].value.as< Ast* > ()); }
#line 1774 "bison.cc" // lalr1.cc:859
    break;

  case 81:
#line 477 "bison.yy" // lalr1.cc:859
    { Ast *hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::CONTINUE, hidden); }
#line 1781 "bison.cc" // lalr1.cc:859
    break;

  case 82:
#line 480 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::THROW, yystack_[1].value.as< Ast* > ()); }
#line 1787 "bison.cc" // lalr1.cc:859
    break;

  case 83:
#line 484 "bison.yy" // lalr1.cc:859
    { ListAst *body = yystack_[0].value.as< Ast* > ()->bodify();
				  yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::WHILE, yystack_[2].value.as< Ast* > (), body); }
#line 1794 "bison.cc" // lalr1.cc:859
    break;

  case 84:
#line 488 "bison.yy" // lalr1.cc:859
    { ListAst *body = yystack_[5].value.as< Ast* > ()->bodify();
				  yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DO_WHILE, body, yystack_[2].value.as< Ast* > ()); }
#line 1801 "bison.cc" // lalr1.cc:859
    break;

  case 85:
#line 492 "bison.yy" // lalr1.cc:859
    { ListAst *body = yystack_[0].value.as< Ast* > ()->bodify();
				  yylhs.value.as< Ast* > () = new FixSizeAst<4>(Ast::Type::FOR, 
										 yystack_[5].value.as< Ast* > (), yystack_[4].value.as< Ast* > (), yystack_[2].value.as< Ast* > (), body); }
#line 1809 "bison.cc" // lalr1.cc:859
    break;

  case 86:
#line 498 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1815 "bison.cc" // lalr1.cc:859
    break;

  case 87:
#line 500 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[1].value.as< Ast* > (); }
#line 1821 "bison.cc" // lalr1.cc:859
    break;

  case 88:
#line 510 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::TRY_LIST); 
				  yylhs.value.as< ListAst* > ()->append(yystack_[2].value.as< ListAst* > ());
				  size_t ct = yystack_[0].value.as< ListAst* > ()->size();
				  for (size_t i = 0; i < ct; i++)
				  	  yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< ListAst* > ()->remove(0));
				  delete yystack_[0].value.as< ListAst* > (); }
#line 1832 "bison.cc" // lalr1.cc:859
    break;

  case 89:
#line 517 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::TRY_LIST); 
				  yylhs.value.as< ListAst* > ()->append(yystack_[6].value.as< ListAst* > ());
				  size_t ct = yystack_[4].value.as< ListAst* > ()->size();
				  for (size_t i = 0; i < ct; i++)
				  	  yylhs.value.as< ListAst* > ()->append(yystack_[4].value.as< ListAst* > ()->remove(0));
				  delete yystack_[4].value.as< ListAst* > (); 
				  yylhs.value.as< ListAst* > ()->append(yystack_[1].value.as< ListAst* > ()); }
#line 1844 "bison.cc" // lalr1.cc:859
    break;

  case 90:
#line 528 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::IF_LIST); }
#line 1850 "bison.cc" // lalr1.cc:859
    break;

  case 91:
#line 530 "bison.yy" // lalr1.cc:859
    { yystack_[1].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); 
				  yylhs.value.as< ListAst* > () = yystack_[1].value.as< ListAst* > (); }
#line 1857 "bison.cc" // lalr1.cc:859
    break;

  case 92:
#line 535 "bison.yy" // lalr1.cc:859
    { Ast *decl = new FixSizeAst<2>(Ast::Type::DECL_VAR, 
												yystack_[5].value.as< Ast* > (), yystack_[4].value.as< Ast* > ());
				  yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::CATCH, decl, yystack_[1].value.as< ListAst* > ()); }
#line 1865 "bison.cc" // lalr1.cc:859
    break;

  case 93:
#line 539 "bison.yy" // lalr1.cc:859
    { Ast *e = new ScalarAst(Ast::Type::IDENT, "e");
				  Ast *decl = new FixSizeAst<2>(Ast::Type::DECL_VAR,
												yystack_[4].value.as< Ast* > (), e);
				  yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::CATCH, decl, yystack_[1].value.as< ListAst* > ()); }
#line 1874 "bison.cc" // lalr1.cc:859
    break;

  case 94:
#line 548 "bison.yy" // lalr1.cc:859
    { Ast *cond = new FixSizeAst<2>(Ast::Type::IF_CONDBODY,
												yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()->bodify()); 
				  yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::IF_LIST);
				  yylhs.value.as< ListAst* > ()->append(cond); }
#line 1883 "bison.cc" // lalr1.cc:859
    break;

  case 95:
#line 553 "bison.yy" // lalr1.cc:859
    { Ast *cond = new FixSizeAst<2>(Ast::Type::IF_CONDBODY,
												yystack_[4].value.as< Ast* > (), yystack_[2].value.as< Ast* > ()->bodify()); 
				  yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::IF_LIST);
				  yylhs.value.as< ListAst* > ()->append(cond); 
				  if (yystack_[0].value.as< Ast* > ()->getType() == Ast::Type::IF_LIST) {
					  ListAst &tail = yystack_[0].value.as< Ast* > ()->asList();
					  size_t size = tail.size();
					  for (size_t i = 0; i < size; i++)
						  yylhs.value.as< ListAst* > ()->append(tail.remove(0));
				  } else {
					  yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()->bodify());
				  } }
#line 1900 "bison.cc" // lalr1.cc:859
    break;

  case 96:
#line 569 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1906 "bison.cc" // lalr1.cc:859
    break;

  case 97:
#line 573 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1912 "bison.cc" // lalr1.cc:859
    break;

  case 98:
#line 575 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASSIGN, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1918 "bison.cc" // lalr1.cc:859
    break;

  case 99:
#line 577 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_ADD, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1924 "bison.cc" // lalr1.cc:859
    break;

  case 100:
#line 579 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_SUB, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1930 "bison.cc" // lalr1.cc:859
    break;

  case 101:
#line 581 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_MUL, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1936 "bison.cc" // lalr1.cc:859
    break;

  case 102:
#line 583 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_DIV, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1942 "bison.cc" // lalr1.cc:859
    break;

  case 103:
#line 585 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_MOD, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1948 "bison.cc" // lalr1.cc:859
    break;

  case 104:
#line 587 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_AND, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1954 "bison.cc" // lalr1.cc:859
    break;

  case 105:
#line 589 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_XOR, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1960 "bison.cc" // lalr1.cc:859
    break;

  case 106:
#line 591 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_OR, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1966 "bison.cc" // lalr1.cc:859
    break;

  case 107:
#line 593 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_SHL, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1972 "bison.cc" // lalr1.cc:859
    break;

  case 108:
#line 595 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_SHR, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1978 "bison.cc" // lalr1.cc:859
    break;

  case 109:
#line 597 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASS_SHRA, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1984 "bison.cc" // lalr1.cc:859
    break;

  case 110:
#line 601 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1990 "bison.cc" // lalr1.cc:859
    break;

  case 111:
#line 603 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<3>(Ast::Type::QUESTION, yystack_[4].value.as< Ast* > (), yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1996 "bison.cc" // lalr1.cc:859
    break;

  case 112:
#line 607 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2002 "bison.cc" // lalr1.cc:859
    break;

  case 113:
#line 609 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::LOGIC_OR_BOP_LIST, 
									  yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), BopListAst::DEFAULT); }
#line 2009 "bison.cc" // lalr1.cc:859
    break;

  case 114:
#line 614 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2015 "bison.cc" // lalr1.cc:859
    break;

  case 115:
#line 616 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::LOGIC_AND_BOP_LIST, 
									  yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), BopListAst::DEFAULT); }
#line 2022 "bison.cc" // lalr1.cc:859
    break;

  case 116:
#line 621 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2028 "bison.cc" // lalr1.cc:859
    break;

  case 117:
#line 623 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::BIT_OR, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2034 "bison.cc" // lalr1.cc:859
    break;

  case 118:
#line 627 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2040 "bison.cc" // lalr1.cc:859
    break;

  case 119:
#line 629 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::BIT_XOR, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2046 "bison.cc" // lalr1.cc:859
    break;

  case 120:
#line 633 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2052 "bison.cc" // lalr1.cc:859
    break;

  case 121:
#line 635 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::BIT_AND, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2058 "bison.cc" // lalr1.cc:859
    break;

  case 122:
#line 639 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2064 "bison.cc" // lalr1.cc:859
    break;

  case 123:
#line 641 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::EQ, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2070 "bison.cc" // lalr1.cc:859
    break;

  case 124:
#line 643 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::NEQ, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2076 "bison.cc" // lalr1.cc:859
    break;

  case 125:
#line 647 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2082 "bison.cc" // lalr1.cc:859
    break;

  case 126:
#line 649 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::LT, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2088 "bison.cc" // lalr1.cc:859
    break;

  case 127:
#line 651 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::GT, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2094 "bison.cc" // lalr1.cc:859
    break;

  case 128:
#line 653 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::LEQ, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2100 "bison.cc" // lalr1.cc:859
    break;

  case 129:
#line 655 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::GEQ, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2106 "bison.cc" // lalr1.cc:859
    break;

  case 130:
#line 657 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::INSTANCEOF, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2112 "bison.cc" // lalr1.cc:859
    break;

  case 131:
#line 661 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2118 "bison.cc" // lalr1.cc:859
    break;

  case 132:
#line 663 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::SHL, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2124 "bison.cc" // lalr1.cc:859
    break;

  case 133:
#line 665 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::SHR, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2130 "bison.cc" // lalr1.cc:859
    break;

  case 134:
#line 667 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::SHRA, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2136 "bison.cc" // lalr1.cc:859
    break;

  case 135:
#line 672 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2142 "bison.cc" // lalr1.cc:859
    break;

  case 136:
#line 674 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::ADD_BOP_LIST, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), 
									  BopListAst::ADD); }
#line 2149 "bison.cc" // lalr1.cc:859
    break;

  case 137:
#line 677 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::ADD_BOP_LIST, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), 
									  BopListAst::SUB); }
#line 2156 "bison.cc" // lalr1.cc:859
    break;

  case 138:
#line 682 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2162 "bison.cc" // lalr1.cc:859
    break;

  case 139:
#line 684 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::MUL_BOP_LIST, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), 
									  BopListAst::MUL); }
#line 2169 "bison.cc" // lalr1.cc:859
    break;

  case 140:
#line 687 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::MUL_BOP_LIST, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), 
									  BopListAst::DIV); }
#line 2176 "bison.cc" // lalr1.cc:859
    break;

  case 141:
#line 690 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > ()=yystack_[2].value.as< Ast* > (); }
#line 2182 "bison.cc" // lalr1.cc:859
    break;

  case 142:
#line 694 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2188 "bison.cc" // lalr1.cc:859
    break;

  case 143:
#line 696 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::CAST, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2194 "bison.cc" // lalr1.cc:859
    break;

  case 144:
#line 698 "bison.yy" // lalr1.cc:859
    { Ast *d = new BopListAst(yystack_[3].value.as< Ast* > (), yystack_[2].value.as< int > ());
				  yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::CAST, d, yystack_[0].value.as< Ast* > ()); }
#line 2201 "bison.cc" // lalr1.cc:859
    break;

  case 145:
#line 701 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::CAST, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 2207 "bison.cc" // lalr1.cc:859
    break;

  case 146:
#line 705 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2213 "bison.cc" // lalr1.cc:859
    break;

  case 147:
#line 707 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(yystack_[1].value.as< Ast* > (), yystack_[0].value.as< int > ()); }
#line 2219 "bison.cc" // lalr1.cc:859
    break;

  case 148:
#line 711 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::PRE_INC, yystack_[0].value.as< Ast* > ()); }
#line 2225 "bison.cc" // lalr1.cc:859
    break;

  case 149:
#line 713 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::PRE_DEC, yystack_[0].value.as< Ast* > ()); }
#line 2231 "bison.cc" // lalr1.cc:859
    break;

  case 150:
#line 715 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::UNARY_PLUS, yystack_[0].value.as< Ast* > ()); }
#line 2237 "bison.cc" // lalr1.cc:859
    break;

  case 151:
#line 717 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::UNARY_MINUS, yystack_[0].value.as< Ast* > ()); }
#line 2243 "bison.cc" // lalr1.cc:859
    break;

  case 152:
#line 719 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2249 "bison.cc" // lalr1.cc:859
    break;

  case 153:
#line 723 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2255 "bison.cc" // lalr1.cc:859
    break;

  case 154:
#line 725 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::BIT_NOT, yystack_[0].value.as< Ast* > ()); }
#line 2261 "bison.cc" // lalr1.cc:859
    break;

  case 155:
#line 727 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::LOGIC_NOT, yystack_[0].value.as< Ast* > ()); }
#line 2267 "bison.cc" // lalr1.cc:859
    break;

  case 156:
#line 731 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2273 "bison.cc" // lalr1.cc:859
    break;

  case 157:
#line 733 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2279 "bison.cc" // lalr1.cc:859
    break;

  case 158:
#line 737 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::POST_INC, yystack_[1].value.as< Ast* > ()); }
#line 2285 "bison.cc" // lalr1.cc:859
    break;

  case 159:
#line 739 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::POST_DEC, yystack_[1].value.as< Ast* > ()); }
#line 2291 "bison.cc" // lalr1.cc:859
    break;

  case 160:
#line 743 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2297 "bison.cc" // lalr1.cc:859
    break;

  case 161:
#line 745 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2303 "bison.cc" // lalr1.cc:859
    break;

  case 162:
#line 750 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2309 "bison.cc" // lalr1.cc:859
    break;

  case 163:
#line 752 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2315 "bison.cc" // lalr1.cc:859
    break;

  case 164:
#line 754 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2321 "bison.cc" // lalr1.cc:859
    break;

  case 165:
#line 758 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[1].value.as< Ast* > (); }
#line 2327 "bison.cc" // lalr1.cc:859
    break;

  case 166:
#line 760 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2333 "bison.cc" // lalr1.cc:859
    break;

  case 167:
#line 764 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new ScalarAst(Ast::Type::NUMBER, yystack_[0].value.as< std::string > ()); }
#line 2339 "bison.cc" // lalr1.cc:859
    break;

  case 168:
#line 766 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new ScalarAst(Ast::Type::STRING, yystack_[0].value.as< std::string > ()); }
#line 2345 "bison.cc" // lalr1.cc:859
    break;

  case 169:
#line 768 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2351 "bison.cc" // lalr1.cc:859
    break;

  case 170:
#line 770 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2357 "bison.cc" // lalr1.cc:859
    break;

  case 171:
#line 772 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::DOT_BOP_LIST, yystack_[3].value.as< Ast* > (), yystack_[1].value.as< Ast* > (), 
									  BopListAst::ARR); }
#line 2364 "bison.cc" // lalr1.cc:859
    break;

  case 172:
#line 775 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::DOT_BOP_LIST, yystack_[3].value.as< Ast* > (), yystack_[1].value.as< Ast* > (), 
									  BopListAst::ARR); }
#line 2371 "bison.cc" // lalr1.cc:859
    break;

  case 173:
#line 780 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::DOT_BOP_LIST, yystack_[3].value.as< Ast* > (), yystack_[1].value.as< ListAst* > (), 
									  BopListAst::CALL); }
#line 2378 "bison.cc" // lalr1.cc:859
    break;

  case 174:
#line 785 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > ()=yystack_[2].value.as< Ast* > (); }
#line 2384 "bison.cc" // lalr1.cc:859
    break;

  case 175:
#line 787 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > ()=yystack_[2].value.as< Ast* > (); }
#line 2390 "bison.cc" // lalr1.cc:859
    break;

  case 176:
#line 791 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2396 "bison.cc" // lalr1.cc:859
    break;

  case 177:
#line 800 "bison.yy" // lalr1.cc:859
    { Ast* hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<3>(Ast::Type::NEW_CLASS, 
										 yystack_[3].value.as< Ast* > (), yystack_[1].value.as< ListAst* > (), hidden); }
#line 2404 "bison.cc" // lalr1.cc:859
    break;

  case 178:
#line 804 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<3>(Ast::Type::NEW_CLASS, 
										 yystack_[6].value.as< Ast* > (), yystack_[4].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); }
#line 2411 "bison.cc" // lalr1.cc:859
    break;

  case 179:
#line 808 "bison.yy" // lalr1.cc:859
    { Ast *hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  yystack_[1].value.as< Ast* > ()->asBopList().addDims(yystack_[0].value.as< int > ());
				  yylhs.value.as< Ast* > () = new FixSizeAst<3>(Ast::Type::NEW_ARRAY,
										 yystack_[2].value.as< Ast* > (), yystack_[1].value.as< Ast* > (), hidden); }
#line 2420 "bison.cc" // lalr1.cc:859
    break;

  case 180:
#line 813 "bison.yy" // lalr1.cc:859
    { Ast *hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  yylhs.value.as< Ast* > () = new FixSizeAst<3>(Ast::Type::NEW_ARRAY,
										 yystack_[1].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), hidden); }
#line 2428 "bison.cc" // lalr1.cc:859
    break;

  case 181:
#line 817 "bison.yy" // lalr1.cc:859
    { Ast *hidden = new ScalarAst(Ast::Type::HIDDEN, "");
				  Ast *dimsA = BopListAst::makeDims(yystack_[0].value.as< int > ());
				  yylhs.value.as< Ast* > () = new FixSizeAst<3>(Ast::Type::NEW_ARRAY,
										 yystack_[1].value.as< Ast* > (), dimsA, hidden); }
#line 2437 "bison.cc" // lalr1.cc:859
    break;

  case 182:
#line 824 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = BopListAst::makeDims(yystack_[1].value.as< Ast* > ()); }
#line 2443 "bison.cc" // lalr1.cc:859
    break;

  case 183:
#line 826 "bison.yy" // lalr1.cc:859
    { yystack_[3].value.as< Ast* > ()->asBopList().addDims(yystack_[1].value.as< Ast* > ());
				  yylhs.value.as< Ast* > () = yystack_[3].value.as< Ast* > (); }
#line 2450 "bison.cc" // lalr1.cc:859
    break;

  case 184:
#line 831 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2456 "bison.cc" // lalr1.cc:859
    break;

  case 185:
#line 833 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2462 "bison.cc" // lalr1.cc:859
    break;

  case 186:
#line 835 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2468 "bison.cc" // lalr1.cc:859
    break;

  case 187:
#line 839 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 2474 "bison.cc" // lalr1.cc:859
    break;

  case 188:
#line 841 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new BopListAst(Ast::Type::DOT_BOP_LIST, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), 
									  BopListAst::DOT); }
#line 2481 "bison.cc" // lalr1.cc:859
    break;

  case 189:
#line 846 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::NAME_LIST);
				  yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); }
#line 2488 "bison.cc" // lalr1.cc:859
    break;

  case 190:
#line 849 "bison.yy" // lalr1.cc:859
    { yystack_[2].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[2].value.as< ListAst* > (); }
#line 2494 "bison.cc" // lalr1.cc:859
    break;

  case 191:
#line 853 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > ()= new ScalarAst(Ast::Type::IDENT,"this"); }
#line 2500 "bison.cc" // lalr1.cc:859
    break;

  case 192:
#line 855 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > ()= new ScalarAst(Ast::Type::IDENT,"super"); }
#line 2506 "bison.cc" // lalr1.cc:859
    break;

  case 193:
#line 857 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > ()= new ScalarAst(Ast::Type::IDENT,"null"); }
#line 2512 "bison.cc" // lalr1.cc:859
    break;

  case 194:
#line 861 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< int > () = 1; }
#line 2518 "bison.cc" // lalr1.cc:859
    break;

  case 195:
#line 863 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< int > () = yystack_[1].value.as< int > () + 1; }
#line 2524 "bison.cc" // lalr1.cc:859
    break;


#line 2528 "bison.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  BisonParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  BisonParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int BisonParser::yypact_ninf_ = -305;

  const short int BisonParser::yytable_ninf_ = -187;

  const short int
  BisonParser::yypact_[] =
  {
    -305,   470,  -305,   -62,   -62,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  1879,  -305,  -305,  -305,     9,
      12,   -62,   -62,  -305,   -62,   -62,  -305,   -62,   -62,  -305,
      35,    46,  -305,   -24,    29,    82,  1401,    66,   126,  1462,
     -62,   -62,  -305,   -62,   -62,  -305,   -62,   -62,   -62,  -305,
    -305,  -305,  -305,   -62,   -44,  -305,    30,  -305,  -305,   -24,
     -62,  -305,  -305,  -305,    72,   197,  1472,    88,   216,  1482,
    -305,   218,   -24,  1543,  1553,  -305,   140,   167,    -8,  -305,
     -31,   -62,   221,  1563,  1624,   -62,  -305,  -305,  -305,   -62,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,   -62,  -305,   -62,
     528,   -17,   -31,  -305,   175,   187,    91,  -305,  -305,  -305,
     232,  1634,  1644,   236,  1705,  1715,  1725,  -305,    76,  -305,
    -305,  -305,  -305,    99,   528,   528,   215,  1299,  1299,  1299,
    1299,  -305,  -305,  -305,  -305,  -305,    34,   -21,    15,    31,
       0,   231,   102,   129,   100,   169,  -305,  1405,  -305,   230,
      55,  -305,    59,    49,    73,  -305,  -305,  -305,  -305,   141,
     112,   148,    99,   -62,    65,   160,  -305,  -305,  -305,   -17,
    1786,  -305,  -305,  -305,  -305,  -305,  -305,  -305,   -49,  -305,
    -305,  -305,   -44,   162,   164,   183,   528,  -305,  -305,  -305,
    -305,   528,   528,   528,   528,   528,   528,   528,   528,   528,
     528,   528,   528,   528,   528,   528,   528,   528,   528,   528,
     528,   528,   528,   528,   528,   528,   528,   528,   528,   528,
     528,   528,   528,   528,  -305,  -305,   -62,   -62,   528,   528,
     528,   -62,  -305,    -9,   -17,   178,  -305,  1796,  1806,   528,
     528,   120,   -31,   -31,   522,   528,   -32,   186,   193,   -21,
      15,    31,     0,   231,   102,   102,   129,   129,   129,   129,
     129,   100,   100,   100,   169,   169,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,   177,   188,   205,  -305,   219,  -305,   -62,
    -305,  -305,    22,  -305,  -305,   226,   203,   528,   -31,  -305,
    -305,   528,  -305,   528,  -305,  -305,   528,  -305,   256,   612,
     -62,  -305,  -305,   224,   247,  -305,  -305,  -305,  -305,   229,
    1323,   235,   255,   285,   -46,   -43,   -62,   261,  -305,  -305,
     -62,    30,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
     270,   104,   264,   691,  -305,  -305,   770,   528,   329,  1391,
     528,  -305,   283,  -305,   284,  -305,   286,   287,  -305,   849,
     -62,  -305,  -305,  -305,  1867,  -305,   281,   288,  -305,   528,
     290,   289,  -305,  -305,  -305,  -305,   928,  -305,  1007,  -305,
    1323,   528,   293,  -305,  1323,  -305,  -305,  -305,   291,   528,
     341,   299,   301,   294,  1323,   296,   302,  -305,  -305,  1323,
    -305,    99,  -305,  -305,   -38,  1086,   303,   308,  -305,  -305,
     306,  1165,  -305,  -305,  1244,  -305
  };

  const unsigned char
  BisonParser::yydefact_[] =
  {
      19,     0,     1,     0,     0,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,     0,     7,    20,    61,     0,
       0,     0,     0,     8,     0,     0,    37,     0,     0,    37,
       0,     0,   187,   189,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,    37,     0,     0,     0,    37,
      37,    21,     6,     0,     2,     4,     0,    38,    39,     5,
       0,    37,    37,    29,     0,     0,     0,     0,     0,     0,
     188,     0,   190,     0,     0,    74,     0,     0,    72,   194,
       3,     0,     0,     0,     0,     0,    37,    37,    25,     0,
      37,    37,    33,    37,    22,    23,    67,     0,    66,     0,
       0,    44,    75,   195,     0,     0,    72,    37,    30,    31,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    70,
     193,   191,   192,     0,     0,     0,     0,     0,     0,     0,
       0,   167,   168,    73,    96,    97,   110,   112,   114,   116,
     118,   120,   122,   125,   131,   135,   138,   142,   152,   153,
     156,   157,   161,   162,   166,   169,   170,   164,   176,     0,
     160,   163,     0,     0,     0,    45,    46,    69,    68,    44,
       0,    37,    26,    27,    37,    34,    35,    24,     0,   151,
     142,   150,   146,     0,     0,   160,     0,   155,   154,   148,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,   159,     0,     0,     0,    62,
       0,     0,    48,     0,     0,     0,    32,     0,     0,     0,
      62,   180,   181,   147,   165,     0,     0,     0,     0,   113,
     115,   117,   119,   121,   123,   124,   130,   126,   127,   128,
     129,   132,   133,   134,   137,   136,   139,   140,   141,    98,
     100,    99,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   175,   174,     0,     0,    63,    64,     0,    49,     0,
      50,    47,     0,    28,    36,     0,     0,     0,   179,   145,
     143,     0,   165,     0,   172,   173,     0,   171,     0,     0,
       0,    50,   182,   177,     0,   144,   111,    65,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    40,
       0,     0,    51,    53,    54,    55,    56,    57,    59,    58,
       0,   160,     0,     0,    37,   183,     0,     0,     0,     0,
       0,    77,     0,    79,     0,    81,     0,     0,    50,     0,
       0,    52,    50,    41,     0,    42,     0,     0,    86,     0,
       0,     0,    76,    78,    80,    82,     0,    60,     0,   178,
       0,     0,     0,    87,     0,    90,    43,    83,     0,     0,
      94,    88,     0,     0,     0,     0,     0,    91,    84,     0,
      95,     0,    50,    85,     0,     0,     0,     0,    89,    50,
       0,     0,    50,    93,     0,    92
  };

  const short int
  BisonParser::yypgoto_[] =
  {
    -305,    53,   248,   246,    -1,   -14,  -305,  -305,   -16,  -305,
     207,  -305,   144,  -222,  -304,     1,   134,  -305,   185,   -75,
     -78,    54,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,   -98,   435,    78,  -305,   190,   192,   194,   195,   184,
     132,    67,    52,   127,   -41,  -305,   315,   145,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
      77,   170,  -305,   -29
  };

  const short int
  BisonParser::yydefgoto_[] =
  {
      -1,    53,    54,    55,    56,    16,     1,    17,    36,    57,
     164,   165,   166,   309,   332,    32,   284,   285,    58,    76,
      77,   118,   334,   335,   336,   337,   369,   338,   391,   397,
     339,   340,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   184,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   241,   159,
     160,    34,   161,   102
  };

  const short int
  BisonParser::yytable_[] =
  {
      15,    23,   133,   105,    19,    20,   104,   289,   353,   162,
     239,   355,   240,    39,    24,    25,   348,    27,    28,   117,
      18,   119,    30,    31,   406,    80,    66,   100,   183,    69,
     301,    46,    79,    73,    74,    47,    18,    79,   310,    18,
      40,    41,    23,   193,    18,    83,    84,    70,   290,   103,
     103,    43,    44,   101,    75,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    18,    26,   196,    52,    29,
     111,   112,    60,    79,   114,   115,   387,   116,    85,   311,
     390,   194,    75,   179,   181,    48,    49,   191,   247,   343,
     400,   170,    42,   248,    89,   403,   346,   192,    75,   195,
      75,    33,    33,    45,    33,    33,   359,    78,   228,    81,
     226,   100,    18,    59,   227,    52,    59,    33,    33,   199,
      33,    33,    48,    61,    33,    72,   100,   233,    48,    86,
     283,   286,   287,    59,  -184,   106,   376,    33,    48,    50,
     378,   295,   286,    59,    48,    90,    59,   207,   208,   242,
      59,    59,   169,   243,   163,   237,   246,    79,   238,    46,
      59,    59,    33,   230,   232,  -186,    33,    46,   266,   267,
     268,   230,    79,  -186,   200,   201,   202,   203,    59,   297,
     405,    18,    48,    62,    52,    -5,    -5,   411,    59,    59,
     414,    59,    59,    59,    96,    35,    97,    37,    38,   314,
      59,    79,   229,   185,   300,   204,   205,   206,   317,  -185,
      64,    65,   298,    67,    68,   231,   234,    71,   209,   210,
     211,    98,   163,    99,   244,   352,   245,   281,   282,   167,
      82,    97,   288,   120,   121,   122,   123,   304,    46,    59,
     292,   168,   230,    99,  -186,   303,    59,    59,   302,   366,
     305,   370,   371,    48,    87,   110,   261,   262,   263,   113,
     315,   306,   124,   125,    79,   313,   256,   257,   258,   259,
     260,   382,    48,    91,    48,    93,   126,    48,   107,   307,
     127,   344,   105,   388,   128,   104,   312,   163,    48,   171,
     347,   393,    48,   174,   129,   130,   349,    18,   131,   132,
      52,   197,   198,   120,   121,   122,   123,   345,   331,   224,
     225,    59,    48,   318,    59,    59,   350,    23,   358,   331,
      48,   362,   395,   396,   361,   354,   356,   357,   364,   254,
     255,    75,   124,   125,   264,   265,   367,   372,   373,   351,
     374,   375,   331,   380,   383,   331,   126,   389,   331,   381,
     127,   384,   394,   392,   128,   398,   399,   401,   331,   402,
     409,    75,   330,   412,   129,   130,    33,    18,   131,   132,
     410,   178,   182,   330,   296,   331,   235,   331,   291,   331,
     253,   316,   249,   331,   360,   250,   341,    33,   251,   299,
     252,     0,     0,   331,     0,     0,   330,   341,   331,   330,
       0,     0,   330,     0,   331,   407,     0,     0,    59,     0,
     331,     0,   330,   331,     0,     0,     0,     0,     0,     0,
     341,     0,     0,   341,     0,     0,   341,     0,     0,   330,
       0,   330,     0,   330,     0,     0,   341,   330,     0,   180,
     180,    59,   187,   188,   189,   190,     0,   330,     0,     0,
       0,     0,   330,   341,   404,   341,     0,   341,   330,   308,
       0,   341,     0,     0,   330,     0,     0,   330,     0,     0,
       2,   341,     0,     3,     4,     0,   341,     0,    59,     0,
     342,     0,   341,     0,     0,     0,     0,     0,   341,     0,
       0,   341,     0,     0,   333,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,   333,     0,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,     0,   333,     0,
       0,   333,     0,     0,   368,     0,     0,     0,     0,     0,
     120,   121,   122,   123,   333,     0,   120,   121,   122,   123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   333,     0,   333,     0,   333,     0,     0,     0,   333,
       0,     0,     0,     0,     0,   124,   125,     0,     0,   333,
       0,     0,     0,   186,   333,     0,     0,   127,     0,   126,
     333,   128,     0,   127,     0,     0,   333,   128,     0,   333,
       0,     0,     0,     0,    18,   131,   132,   129,   130,     0,
      18,   131,   132,     0,     0,     0,   180,     0,   180,   319,
     320,   321,   322,     0,   323,   324,   325,   326,     0,     0,
     120,   121,   122,   123,   327,     0,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   124,
     125,     0,     0,     0,     0,     0,     0,     0,     0,   328,
     329,     0,     0,   126,     0,     0,     0,   127,     0,     0,
       0,   128,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   129,   130,     0,    18,   131,   132,    52,   319,   320,
     321,   322,     0,   323,   324,   325,   326,     0,     0,   120,
     121,   122,   123,   327,     0,     0,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   124,   125,
       0,     0,     0,     0,     0,     0,     0,     0,   328,   363,
       0,     0,   126,     0,     0,     0,   127,     0,     0,     0,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     129,   130,     0,    18,   131,   132,    52,   319,   320,   321,
     322,     0,   323,   324,   325,   326,     0,     0,   120,   121,
     122,   123,   327,     0,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   124,   125,     0,
       0,     0,     0,     0,     0,     0,     0,   328,   365,     0,
       0,   126,     0,     0,     0,   127,     0,     0,     0,   128,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   129,
     130,     0,    18,   131,   132,    52,   319,   320,   321,   322,
       0,   323,   324,   325,   326,     0,     0,   120,   121,   122,
     123,   327,     0,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   124,   125,     0,     0,
       0,     0,     0,     0,     0,     0,   328,   377,     0,     0,
     126,     0,     0,     0,   127,     0,     0,     0,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   129,   130,
       0,    18,   131,   132,    52,   319,   320,   321,   322,     0,
     323,   324,   325,   326,     0,     0,   120,   121,   122,   123,
     327,     0,     0,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   124,   125,     0,     0,     0,
       0,     0,     0,     0,     0,   328,   385,     0,     0,   126,
       0,     0,     0,   127,     0,     0,     0,   128,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   129,   130,     0,
      18,   131,   132,    52,   319,   320,   321,   322,     0,   323,
     324,   325,   326,     0,     0,   120,   121,   122,   123,   327,
       0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   124,   125,     0,     0,     0,     0,
       0,     0,     0,     0,   328,   386,     0,     0,   126,     0,
       0,     0,   127,     0,     0,     0,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   129,   130,     0,    18,
     131,   132,    52,   319,   320,   321,   322,     0,   323,   324,
     325,   326,     0,     0,   120,   121,   122,   123,   327,     0,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   124,   125,     0,     0,     0,     0,     0,
       0,     0,     0,   328,   408,     0,     0,   126,     0,     0,
       0,   127,     0,     0,     0,   128,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   129,   130,     0,    18,   131,
     132,    52,   319,   320,   321,   322,     0,   323,   324,   325,
     326,     0,     0,   120,   121,   122,   123,   327,     0,     0,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,   125,     0,     0,     0,     0,     0,     0,
       0,     0,   328,   413,     0,     0,   126,     0,     0,     0,
     127,     0,     0,     0,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   129,   130,     0,    18,   131,   132,
      52,   319,   320,   321,   322,     0,   323,   324,   325,   326,
       0,     0,   120,   121,   122,   123,   327,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   124,   125,     0,     0,     0,     0,     0,     0,     0,
       0,   328,   415,     0,     0,   126,     0,     0,     0,   127,
       0,     0,     0,   128,     0,     0,     0,   120,   121,   122,
     123,     0,     0,   129,   130,     0,    18,   131,   132,    52,
     319,   320,   321,   322,     0,   323,   324,   325,   326,     0,
       0,   120,   121,   122,   123,   327,   124,   125,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,     0,     0,
     186,     0,     0,     0,   127,     0,     0,     0,   128,     0,
     124,   125,     0,     0,     0,     0,     0,     0,   129,   130,
     328,    18,   131,   132,   126,     0,     0,     0,   127,     0,
       0,     0,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   129,   130,     0,    18,   131,   132,    52,   120,
     121,   122,   123,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     0,     0,   124,   125,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   126,     0,     0,     0,   127,     0,     0,    51,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     129,   130,     0,    18,   131,   132,    52,     0,     0,     0,
       0,     0,     0,    18,     0,     0,    52,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,    18,     0,     0,    52,     0,     0,
       0,     0,     0,     0,    18,     0,     0,    52,     0,     0,
       0,     0,     0,     0,    18,     0,     0,    52,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   108,     0,     0,     0,    18,     0,     0,    52,     0,
       0,     0,     0,     0,     0,    18,     0,     0,    52,     0,
       0,     0,     0,     0,     0,    18,     0,     0,    52,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,     0,
       0,     0,   109,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,     0,     0,     0,    18,     0,     0,    52,
       0,     0,     0,     0,     0,     0,    18,     0,     0,    52,
       0,     0,     0,     0,     0,     0,    18,     0,     0,    52,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       0,     0,     0,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,     0,     0,     0,    18,     0,     0,
      52,     0,     0,     0,     0,     0,     0,    18,     0,     0,
      52,     0,     0,     0,     0,     0,     0,    18,     0,     0,
      52,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     0,     0,     0,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   294,     0,     0,     0,    18,     0,
       0,    52,     0,     0,     0,     0,     0,     0,    18,     0,
       0,    52,    21,    22,     0,     0,     0,     0,    18,     0,
       0,    52,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,     0,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
       0,     0,    52
  };

  const short int
  BisonParser::yycheck_[] =
  {
       1,    15,   100,    81,     3,     4,    81,    16,    54,    26,
      59,    54,    61,    29,     5,     6,   320,     5,     6,    97,
      82,    99,    21,    22,    62,    54,    42,    35,   126,    45,
      62,    55,    81,    49,    50,     6,    82,    81,    16,    82,
       5,     6,    56,    64,    82,    61,    62,    46,    57,    81,
      81,     5,     6,    61,    53,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    82,    57,    67,    85,    57,
      86,    87,     6,    81,    90,    91,   380,    93,     6,    57,
     384,    66,    81,   124,   125,    56,    57,    53,   186,   311,
     394,   107,    57,   191,     6,   399,   318,    63,    97,    68,
      99,    24,    25,    57,    27,    28,   328,    53,    59,    56,
      55,    35,    82,    36,    55,    85,    39,    40,    41,    17,
      43,    44,    56,    57,    47,    48,    35,    62,    56,    57,
     228,   229,   230,    56,    61,    81,   358,    60,    56,    57,
     362,   239,   240,    66,    56,    57,    69,    47,    48,   178,
      73,    74,    61,   182,   101,   171,   185,    81,   174,    55,
      83,    84,    85,    59,   163,    61,    89,    55,   209,   210,
     211,    59,    81,    61,    72,    73,    74,    75,   101,    59,
     402,    82,    56,    57,    85,    81,    82,   409,   111,   112,
     412,   114,   115,   116,    54,    25,    56,    27,    28,   297,
     123,    81,    61,   126,   245,    76,    77,    78,   306,    61,
      40,    41,   241,    43,    44,   162,    56,    47,    49,    50,
      51,    54,   169,    56,    62,   323,    62,   226,   227,    54,
      60,    56,   231,    18,    19,    20,    21,    60,    55,   162,
      62,    54,    59,    56,    61,    52,   169,   170,    62,   347,
      62,   349,   350,    56,    57,    85,   204,   205,   206,    89,
     301,    56,    47,    48,    81,    62,   199,   200,   201,   202,
     203,   369,    56,    57,    56,    57,    61,    56,    57,    60,
      65,    57,   360,   381,    69,   360,    60,   234,    56,    57,
      61,   389,    56,    57,    79,    80,    61,    82,    83,    84,
      85,    70,    71,    18,    19,    20,    21,    60,   309,    79,
      80,   234,    56,    57,   237,   238,    61,   331,    57,   320,
      56,    57,    23,    24,    54,   324,   325,   326,   344,   197,
     198,   330,    47,    48,   207,   208,     7,    54,    54,    54,
      54,    54,   343,    62,    54,   346,    61,    54,   349,    61,
      65,    62,    11,    62,    69,    54,    62,    61,   359,    57,
      57,   360,   309,    57,    79,    80,   289,    82,    83,    84,
      62,   123,   126,   320,   240,   376,   169,   378,   234,   380,
     196,   303,   192,   384,   331,   193,   309,   310,   194,   244,
     195,    -1,    -1,   394,    -1,    -1,   343,   320,   399,   346,
      -1,    -1,   349,    -1,   405,   404,    -1,    -1,   331,    -1,
     411,    -1,   359,   414,    -1,    -1,    -1,    -1,    -1,    -1,
     343,    -1,    -1,   346,    -1,    -1,   349,    -1,    -1,   376,
      -1,   378,    -1,   380,    -1,    -1,   359,   384,    -1,   124,
     125,   364,   127,   128,   129,   130,    -1,   394,    -1,    -1,
      -1,    -1,   399,   376,   401,   378,    -1,   380,   405,   289,
      -1,   384,    -1,    -1,   411,    -1,    -1,   414,    -1,    -1,
       0,   394,    -1,     3,     4,    -1,   399,    -1,   401,    -1,
     310,    -1,   405,    -1,    -1,    -1,    -1,    -1,   411,    -1,
      -1,   414,    -1,    -1,   309,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,   320,    -1,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,    -1,   343,    -1,
      -1,   346,    -1,    -1,   349,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,   359,    -1,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   376,    -1,   378,    -1,   380,    -1,    -1,    -1,   384,
      -1,    -1,    -1,    -1,    -1,    47,    48,    -1,    -1,   394,
      -1,    -1,    -1,    61,   399,    -1,    -1,    65,    -1,    61,
     405,    69,    -1,    65,    -1,    -1,   411,    69,    -1,   414,
      -1,    -1,    -1,    -1,    82,    83,    84,    79,    80,    -1,
      82,    83,    84,    -1,    -1,    -1,   301,    -1,   303,     7,
       8,     9,    10,    -1,    12,    13,    14,    15,    -1,    -1,
      18,    19,    20,    21,    22,    -1,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    -1,    82,    83,    84,    85,     7,     8,
       9,    10,    -1,    12,    13,    14,    15,    -1,    -1,    18,
      19,    20,    21,    22,    -1,    -1,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    82,    83,    84,    85,     7,     8,     9,
      10,    -1,    12,    13,    14,    15,    -1,    -1,    18,    19,
      20,    21,    22,    -1,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,
      -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    -1,    82,    83,    84,    85,     7,     8,     9,    10,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,
      61,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      -1,    82,    83,    84,    85,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    -1,    -1,    18,    19,    20,    21,
      22,    -1,    -1,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,    61,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,
      82,    83,    84,    85,     7,     8,     9,    10,    -1,    12,
      13,    14,    15,    -1,    -1,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    -1,    -1,    61,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    82,
      83,    84,    85,     7,     8,     9,    10,    -1,    12,    13,
      14,    15,    -1,    -1,    18,    19,    20,    21,    22,    -1,
      -1,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    -1,    -1,    61,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    82,    83,
      84,    85,     7,     8,     9,    10,    -1,    12,    13,    14,
      15,    -1,    -1,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    -1,    82,    83,    84,
      85,     7,     8,     9,    10,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    -1,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    -1,    -1,    61,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    18,    19,    20,
      21,    -1,    -1,    79,    80,    -1,    82,    83,    84,    85,
       7,     8,     9,    10,    -1,    12,    13,    14,    15,    -1,
      -1,    18,    19,    20,    21,    22,    47,    48,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      61,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    -1,
      47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      57,    82,    83,    84,    61,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    -1,    82,    83,    84,    85,    18,
      19,    20,    21,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    47,    48,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    61,    -1,    -1,    -1,    65,    -1,    -1,    58,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    82,    83,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    85,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    82,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    82,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    82,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    82,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      85,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    82,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    85,     3,     4,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    85,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85
  };

  const unsigned char
  BisonParser::yystos_[] =
  {
       0,    93,     0,     3,     4,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    91,    92,    94,    82,   102,
     102,     3,     4,    92,     5,     6,    57,     5,     6,    57,
     102,   102,   102,   147,   148,   148,    95,   148,   148,    95,
       5,     6,    57,     5,     6,    57,    55,     6,    56,    57,
      57,    58,    85,    88,    89,    90,    91,    96,   105,   147,
       6,    57,    57,    58,   148,   148,    95,   148,   148,    95,
     102,   148,   147,    95,    95,   102,   106,   107,   108,    81,
     150,    88,   148,    95,    95,     6,    57,    57,    58,     6,
      57,    57,    58,    57,    58,    58,    54,    56,    54,    56,
      35,    61,   150,    81,   106,   107,   108,    57,    58,    58,
     148,    95,    95,   148,    95,    95,    95,   107,   108,   107,
      18,    19,    20,    21,    47,    48,    61,    65,    69,    79,
      80,    83,    84,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   146,
     147,   149,    26,    88,    97,    98,    99,    54,    54,    61,
      95,    57,    58,    58,    57,    58,    58,    58,    89,   131,
     133,   131,    90,   118,   132,   147,    61,   133,   133,   133,
     133,    53,    63,    64,    66,    68,    67,    70,    71,    17,
      72,    73,    74,    75,    76,    77,    78,    47,    48,    49,
      50,    51,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    79,    80,    55,    55,    59,    61,
      59,    88,   102,    62,    56,    97,    58,    95,    95,    59,
      61,   145,   150,   150,    62,    62,   150,   118,   118,   122,
     123,   124,   125,   126,   127,   127,   128,   128,   128,   128,
     128,   129,   129,   129,   130,   130,   131,   131,   131,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   102,   102,   118,   103,   104,   118,   118,   102,    16,
      57,    99,    62,    58,    58,   118,   103,    59,   150,   134,
     131,    62,    62,    52,    60,    62,    56,    60,   148,   100,
      16,    57,    60,    62,   118,   131,   120,   118,    57,     7,
       8,     9,    10,    12,    13,    14,    15,    22,    57,    58,
      88,    91,   101,   105,   109,   110,   111,   112,   114,   117,
     118,   147,   148,   100,    57,    60,   100,    61,   101,    61,
      61,    54,   118,    54,   102,    54,   102,   102,    57,   100,
      88,    54,    57,    58,    95,    58,   118,     7,   105,   113,
     118,   118,    54,    54,    54,    54,   100,    58,   100,    58,
      62,    61,   118,    54,    62,    58,    58,   101,   118,    54,
     101,   115,    62,   118,    11,    23,    24,   116,    54,    62,
     101,    61,    57,   101,    88,   100,    62,   102,    58,    57,
      62,   100,    57,    58,   100,    58
  };

  const unsigned char
  BisonParser::yyr1_[] =
  {
       0,    87,    88,    88,    89,    89,    90,    91,    91,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    93,
      93,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    95,    95,    95,
      96,    96,    96,    96,    97,    97,    98,    98,    99,    99,
     100,   100,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   102,   103,   103,   104,   104,   105,   105,   105,   105,
     106,   106,   107,   107,   108,   108,   109,   109,   109,   109,
     109,   109,   109,   110,   111,   112,   113,   113,   114,   114,
     115,   115,   116,   116,   117,   117,   118,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     120,   120,   121,   121,   122,   122,   123,   123,   124,   124,
     125,   125,   126,   126,   126,   127,   127,   127,   127,   127,
     127,   128,   128,   128,   128,   129,   129,   129,   130,   130,
     130,   130,   131,   131,   131,   131,   132,   132,   133,   133,
     133,   133,   133,   134,   134,   134,   135,   135,   136,   136,
     137,   137,   138,   138,   138,   139,   139,   140,   140,   140,
     140,   140,   140,   141,   142,   142,   143,   144,   144,   144,
     144,   144,   145,   145,   146,   146,   146,   147,   147,   148,
     148,   149,   149,   149,   150,   150
  };

  const unsigned char
  BisonParser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       2,     5,     7,     7,     9,     6,     8,     8,    10,     5,
       7,     7,     9,     6,     8,     8,    10,     0,     2,     2,
       8,     9,    10,    11,     0,     1,     1,     3,     2,     3,
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     0,     1,     1,     3,     3,     3,     4,     4,
       3,     3,     1,     3,     1,     2,     3,     2,     3,     2,
       3,     2,     3,     5,     7,     8,     1,     2,     5,     9,
       0,     2,     8,     7,     5,     7,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       3,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     4,     5,     4,     1,     2,     2,     2,
       2,     2,     1,     1,     2,     2,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     4,     4,     4,     3,     3,     1,     5,     8,     4,
       3,     3,     3,     4,     1,     1,     1,     1,     3,     1,
       3,     1,     1,     1,     1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BisonParser::yytname_[] =
  {
  "\"EOF\"", "error", "$undefined", "\"class\"", "\"interface\"",
  "\"extends\"", "\"implements\"", "\"while\"", "\"do\"", "\"for\"",
  "\"if\"", "\"else\"", "\"return\"", "\"break\"", "\"continue\"",
  "\"throw\"", "\"throws\"", "\"instanceof\"", "\"null\"", "\"this\"",
  "\"super\"", "\"new\"", "\"try\"", "\"catch\"", "\"finally\"",
  "\"abstract\"", "\"final\"", "\"public\"", "\"protected\"",
  "\"private\"", "\"static\"", "\"transient\"", "\"volatile\"",
  "\"native\"", "\"synchronized\"", "\"=\"", "\"-=\"", "\"+=\"", "\"*=\"",
  "\"/=\"", "\"%=\"", "\"&=\"", "\"^=\"", "\"|=\"", "\"<<=\"", "\">>=\"",
  "\">>>=\"", "\"-\"", "\"+\"", "\"*\"", "\"/\"", "\"%\"", "\":\"",
  "\"?\"", "\";\"", "\".\"", "\",\"", "\"{\"", "\"}\"", "\"[\"", "\"]\"",
  "\"(\"", "\")\"", "\"||\"", "\"&&\"", "\"!\"", "\"|\"", "\"&\"", "\"^\"",
  "\"~\"", "\"==\"", "\"!=\"", "\"<\"", "\">\"", "\"<=\"", "\">=\"",
  "\"<<\"", "\">>\"", "\">>>\"", "\"++\"", "\"--\"", "\"[]\"",
  "\"identifier\"", "\"number\"", "\"string\"", "\"void\"", "\"then\"",
  "$accept", "type_spec", "type_name", "ptype", "modifiers", "modifier",
  "class_list", "class", "member_list", "decl_method", "param_list",
  "param_list_noemp", "decl_param", "stmt_list", "stmt", "ident",
  "arg_list", "arg_list_noemp", "decl_var", "dector_list", "dector",
  "dector_name", "jump_stmt", "while_stmt", "do_while_stmt", "for_stmt",
  "for_init", "try_list", "catch_list", "catch", "if_list", "expr",
  "expr_lv0", "expr_lv1", "expr_lv2", "expr_lv3", "expr_lv4", "expr_lv5",
  "expr_lv6", "expr_lv7", "expr_lv8", "expr_lv9", "expr_lv10", "expr_lv11",
  "expr_lv12", "expr_ptype", "expr_unary", "expr_unary_logic", "expr_pp",
  "expr_post", "expr_prime", "expr_prime_noname", "expr_prime_cx",
  "expr_prime_cx_nude", "expr_call", "expr_field", "expr_new",
  "expr_new_plain", "dim_exprs", "callee", "name", "name_list",
  "special_name", "dims", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  BisonParser::yyrline_[] =
  {
       0,   216,   216,   218,   222,   224,   228,   232,   234,   238,
     240,   242,   244,   246,   248,   250,   252,   254,   256,   260,
     261,   264,   269,   273,   277,   281,   287,   292,   297,   302,
     307,   311,   315,   319,   325,   330,   336,   343,   345,   347,
     351,   355,   361,   365,   372,   374,   377,   380,   384,   386,
     391,   393,   397,   399,   401,   403,   405,   407,   409,   411,
     413,   417,   421,   423,   427,   429,   433,   435,   437,   440,
     445,   448,   452,   454,   458,   460,   464,   466,   469,   471,
     474,   476,   479,   483,   487,   491,   497,   499,   509,   516,
     526,   529,   534,   538,   547,   552,   568,   572,   574,   576,
     578,   580,   582,   584,   586,   588,   590,   592,   594,   596,
     600,   602,   606,   608,   613,   615,   620,   622,   626,   628,
     632,   634,   638,   640,   642,   646,   648,   650,   652,   654,
     656,   660,   662,   664,   666,   671,   673,   676,   681,   683,
     686,   689,   693,   695,   697,   700,   704,   706,   710,   712,
     714,   716,   718,   722,   724,   726,   730,   732,   736,   738,
     742,   744,   749,   751,   753,   757,   759,   763,   765,   767,
     769,   771,   774,   779,   784,   786,   790,   799,   803,   807,
     812,   816,   823,   825,   830,   832,   834,   838,   840,   845,
     848,   852,   854,   856,   860,   862
  };

  // Print the state stack on the debug stream.
  void
  BisonParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  BisonParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 3499 "bison.cc" // lalr1.cc:1167
#line 866 "bison.yy" // lalr1.cc:1168


void yy::BisonParser::error(const location_type& l,
                              const std::string& m)
{
	throw ParseException(l, m);
}

