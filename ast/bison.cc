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
#line 39 "bison.yy" // lalr1.cc:413

	#include "ast/lexproto.h"
	#include "ast/parseexception.h"

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
      case 22: // class
      case 24: // method
      case 27: // decl_param
      case 29: // stmt
      case 30: // expr
      case 31: // name
      case 32: // new_name
      case 35: // decl_stmt
      case 37: // decl_bean
      case 38: // type
      case 39: // return_stmt
      case 40: // while_stmt
        value.move< Ast* > (that.value);
        break;

      case 21: // class_list
      case 23: // method_list
      case 25: // param_list
      case 26: // param_list_noemp
      case 28: // stmt_list
      case 33: // arg_list
      case 34: // arg_list_noemp
      case 36: // decl_bean_list
        value.move< ListAst* > (that.value);
        break;

      case 18: // "identifier"
      case 19: // "number"
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
      case 22: // class
      case 24: // method
      case 27: // decl_param
      case 29: // stmt
      case 30: // expr
      case 31: // name
      case 32: // new_name
      case 35: // decl_stmt
      case 37: // decl_bean
      case 38: // type
      case 39: // return_stmt
      case 40: // while_stmt
        value.copy< Ast* > (that.value);
        break;

      case 21: // class_list
      case 23: // method_list
      case 25: // param_list
      case 26: // param_list_noemp
      case 28: // stmt_list
      case 33: // arg_list
      case 34: // arg_list_noemp
      case 36: // decl_bean_list
        value.copy< ListAst* > (that.value);
        break;

      case 18: // "identifier"
      case 19: // "number"
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
            case 18: // "identifier"

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 374 "bison.cc" // lalr1.cc:636
        break;

      case 19: // "number"

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 381 "bison.cc" // lalr1.cc:636
        break;

      case 21: // class_list

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 388 "bison.cc" // lalr1.cc:636
        break;

      case 22: // class

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 395 "bison.cc" // lalr1.cc:636
        break;

      case 23: // method_list

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 402 "bison.cc" // lalr1.cc:636
        break;

      case 24: // method

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 409 "bison.cc" // lalr1.cc:636
        break;

      case 25: // param_list

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 416 "bison.cc" // lalr1.cc:636
        break;

      case 26: // param_list_noemp

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 423 "bison.cc" // lalr1.cc:636
        break;

      case 27: // decl_param

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 430 "bison.cc" // lalr1.cc:636
        break;

      case 28: // stmt_list

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 437 "bison.cc" // lalr1.cc:636
        break;

      case 29: // stmt

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 444 "bison.cc" // lalr1.cc:636
        break;

      case 30: // expr

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 451 "bison.cc" // lalr1.cc:636
        break;

      case 31: // name

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 458 "bison.cc" // lalr1.cc:636
        break;

      case 32: // new_name

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 465 "bison.cc" // lalr1.cc:636
        break;

      case 33: // arg_list

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 472 "bison.cc" // lalr1.cc:636
        break;

      case 34: // arg_list_noemp

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 479 "bison.cc" // lalr1.cc:636
        break;

      case 35: // decl_stmt

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 486 "bison.cc" // lalr1.cc:636
        break;

      case 36: // decl_bean_list

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ListAst* > (); }
#line 493 "bison.cc" // lalr1.cc:636
        break;

      case 37: // decl_bean

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 500 "bison.cc" // lalr1.cc:636
        break;

      case 38: // type

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 507 "bison.cc" // lalr1.cc:636
        break;

      case 39: // return_stmt

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 514 "bison.cc" // lalr1.cc:636
        break;

      case 40: // while_stmt

#line 92 "bison.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Ast* > (); }
#line 521 "bison.cc" // lalr1.cc:636
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
    #line 29 "bison.yy" // lalr1.cc:741
{
	/* Initialize the initial location. */ 
	/* position.filename is a non-const pointer somehow */
	static std::string s_filename(filename);
	yyla.location.begin.filename = yyla.location.end.filename = &s_filename;
}

#line 642 "bison.cc" // lalr1.cc:741

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
      case 22: // class
      case 24: // method
      case 27: // decl_param
      case 29: // stmt
      case 30: // expr
      case 31: // name
      case 32: // new_name
      case 35: // decl_stmt
      case 37: // decl_bean
      case 38: // type
      case 39: // return_stmt
      case 40: // while_stmt
        yylhs.value.build< Ast* > ();
        break;

      case 21: // class_list
      case 23: // method_list
      case 25: // param_list
      case 26: // param_list_noemp
      case 28: // stmt_list
      case 33: // arg_list
      case 34: // arg_list_noemp
      case 36: // decl_bean_list
        yylhs.value.build< ListAst* > ();
        break;

      case 18: // "identifier"
      case 19: // "number"
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
#line 102 "bison.yy" // lalr1.cc:859
    { /* already newwed as 'result' */ }
#line 783 "bison.cc" // lalr1.cc:859
    break;

  case 3:
#line 103 "bison.yy" // lalr1.cc:859
    { result->append(yystack_[0].value.as< Ast* > ()); }
#line 789 "bison.cc" // lalr1.cc:859
    break;

  case 4:
#line 107 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::CLASS, yystack_[3].value.as< Ast* > (), yystack_[1].value.as< ListAst* > ()); }
#line 795 "bison.cc" // lalr1.cc:859
    break;

  case 5:
#line 111 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::METHOD_LIST); }
#line 801 "bison.cc" // lalr1.cc:859
    break;

  case 6:
#line 113 "bison.yy" // lalr1.cc:859
    { yystack_[1].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[1].value.as< ListAst* > (); }
#line 807 "bison.cc" // lalr1.cc:859
    break;

  case 7:
#line 117 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<3>(Ast::Type::METHOD, yystack_[6].value.as< Ast* > (), yystack_[4].value.as< ListAst* > (), yystack_[1].value.as< ListAst* > ()); }
#line 813 "bison.cc" // lalr1.cc:859
    break;

  case 8:
#line 121 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::DECL_PARAM_LIST); }
#line 819 "bison.cc" // lalr1.cc:859
    break;

  case 9:
#line 123 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = yystack_[0].value.as< ListAst* > (); }
#line 825 "bison.cc" // lalr1.cc:859
    break;

  case 10:
#line 126 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::DECL_PARAM_LIST); 
				  yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); }
#line 832 "bison.cc" // lalr1.cc:859
    break;

  case 11:
#line 129 "bison.yy" // lalr1.cc:859
    { yystack_[2].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[2].value.as< ListAst* > (); }
#line 838 "bison.cc" // lalr1.cc:859
    break;

  case 12:
#line 133 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL_PARAM, yystack_[1].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 844 "bison.cc" // lalr1.cc:859
    break;

  case 13:
#line 137 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::STMT_LIST); }
#line 850 "bison.cc" // lalr1.cc:859
    break;

  case 14:
#line 139 "bison.yy" // lalr1.cc:859
    { yystack_[1].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[1].value.as< ListAst* > (); }
#line 856 "bison.cc" // lalr1.cc:859
    break;

  case 15:
#line 143 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[1].value.as< Ast* > (); }
#line 862 "bison.cc" // lalr1.cc:859
    break;

  case 16:
#line 145 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 868 "bison.cc" // lalr1.cc:859
    break;

  case 17:
#line 147 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 874 "bison.cc" // lalr1.cc:859
    break;

  case 18:
#line 149 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 880 "bison.cc" // lalr1.cc:859
    break;

  case 19:
#line 156 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = TermListAst::makeBop(yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), TermListAst::Op::ADD); }
#line 886 "bison.cc" // lalr1.cc:859
    break;

  case 20:
#line 158 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = TermListAst::makeBop(yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), TermListAst::Op::SUB); }
#line 892 "bison.cc" // lalr1.cc:859
    break;

  case 21:
#line 160 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = TermListAst::makeBop(yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), TermListAst::Op::MUL); }
#line 898 "bison.cc" // lalr1.cc:859
    break;

  case 22:
#line 162 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = TermListAst::makeBop(yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > (), TermListAst::Op::DIV); }
#line 904 "bison.cc" // lalr1.cc:859
    break;

  case 23:
#line 164 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::ASSIGN, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 910 "bison.cc" // lalr1.cc:859
    break;

  case 24:
#line 166 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::CALL, yystack_[3].value.as< Ast* > (), yystack_[1].value.as< ListAst* > ()); }
#line 916 "bison.cc" // lalr1.cc:859
    break;

  case 25:
#line 168 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::PAREN, yystack_[1].value.as< Ast* > ()); }
#line 922 "bison.cc" // lalr1.cc:859
    break;

  case 26:
#line 170 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 928 "bison.cc" // lalr1.cc:859
    break;

  case 27:
#line 172 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new ScalarAst(Ast::Type::NUMBER, yystack_[0].value.as< std::string > ()); }
#line 934 "bison.cc" // lalr1.cc:859
    break;

  case 28:
#line 176 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new ScalarAst(Ast::Type::IDENT, yystack_[0].value.as< std::string > ()); }
#line 940 "bison.cc" // lalr1.cc:859
    break;

  case 29:
#line 180 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new ScalarAst(Ast::Type::IDENT, yystack_[0].value.as< std::string > ()); }
#line 946 "bison.cc" // lalr1.cc:859
    break;

  case 30:
#line 184 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::ARG_LIST); }
#line 952 "bison.cc" // lalr1.cc:859
    break;

  case 31:
#line 186 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = yystack_[0].value.as< ListAst* > (); }
#line 958 "bison.cc" // lalr1.cc:859
    break;

  case 32:
#line 190 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::ARG_LIST); yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); }
#line 964 "bison.cc" // lalr1.cc:859
    break;

  case 33:
#line 192 "bison.yy" // lalr1.cc:859
    { yystack_[2].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[2].value.as< ListAst* > (); }
#line 970 "bison.cc" // lalr1.cc:859
    break;

  case 34:
#line 196 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::DECL, yystack_[2].value.as< Ast* > (), yystack_[1].value.as< ListAst* > ()); }
#line 976 "bison.cc" // lalr1.cc:859
    break;

  case 35:
#line 200 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< ListAst* > () = new ListAst(Ast::Type::DECL_BEAN_LIST); 
				  yylhs.value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); }
#line 983 "bison.cc" // lalr1.cc:859
    break;

  case 36:
#line 203 "bison.yy" // lalr1.cc:859
    { yystack_[2].value.as< ListAst* > ()->append(yystack_[0].value.as< Ast* > ()); yylhs.value.as< ListAst* > () = yystack_[2].value.as< ListAst* > (); }
#line 989 "bison.cc" // lalr1.cc:859
    break;

  case 37:
#line 207 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new DeclBeanAst(yystack_[0].value.as< Ast* > ()); }
#line 995 "bison.cc" // lalr1.cc:859
    break;

  case 38:
#line 209 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new DeclBeanAst(yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1001 "bison.cc" // lalr1.cc:859
    break;

  case 39:
#line 213 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = yystack_[0].value.as< Ast* > (); }
#line 1007 "bison.cc" // lalr1.cc:859
    break;

  case 40:
#line 217 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<1>(Ast::Type::RETURN, yystack_[1].value.as< Ast* > ()); }
#line 1013 "bison.cc" // lalr1.cc:859
    break;

  case 41:
#line 220 "bison.yy" // lalr1.cc:859
    { yylhs.value.as< Ast* > () = new FixSizeAst<2>(Ast::Type::WHILE, yystack_[2].value.as< Ast* > (), yystack_[0].value.as< Ast* > ()); }
#line 1019 "bison.cc" // lalr1.cc:859
    break;


#line 1023 "bison.cc" // lalr1.cc:859
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


  const signed char BisonParser::yypact_ninf_ = -26;

  const signed char BisonParser::yytable_ninf_ = -40;

  const signed char
  BisonParser::yypact_[] =
  {
     -26,    37,   -26,   -16,   -26,   -26,    42,   -26,    33,   -16,
     -26,   -26,    31,    46,   -26,    48,    53,   -26,   -26,   -16,
      60,    46,   -26,   -26,   -26,    39,     6,    51,   -26,     6,
     -26,   -26,    56,    -9,   -26,   -16,   -26,   -26,    66,    59,
       6,    24,     6,     6,     6,     6,     6,   -26,     6,    73,
      27,   -26,   -26,    29,   -26,    76,    45,    45,   -26,   -26,
      76,    67,    64,     6,   -26,   -16,    -5,   -26,     6,    76,
     -26,   -26,    76
  };

  const unsigned char
  BisonParser::yydefact_[] =
  {
       2,     0,     1,     0,     3,    29,     0,     5,     0,     0,
       4,     6,     0,     8,    28,     0,     9,    10,    39,     0,
       0,     0,    12,    13,    11,     0,     0,     0,     7,     0,
      27,    14,     0,    26,    16,     0,    17,    18,     0,    26,
       0,     0,     0,     0,     0,     0,     0,    15,    30,    37,
       0,    35,    40,     0,    25,    23,    20,    19,    21,    22,
      32,     0,    31,     0,    34,     0,     0,    24,     0,    38,
      36,    41,    33
  };

  const signed char
  BisonParser::yypgoto_[] =
  {
     -26,   -26,   -26,   -26,   -26,   -26,   -26,    65,   -26,    19,
     -25,   -13,     7,   -26,   -26,   -26,   -26,    22,   -22,   -26,
     -26
  };

  const signed char
  BisonParser::yydefgoto_[] =
  {
      -1,     1,     4,     8,    11,    15,    16,    17,    25,    31,
      32,    39,    49,    61,    62,    34,    50,    51,    19,    36,
      37
  };

  const signed char
  BisonParser::yytable_[] =
  {
      18,    38,     5,    35,    41,    26,    27,    48,    18,   -39,
       6,    29,    33,    14,    30,    53,    12,    55,    56,    57,
      58,    59,    29,    60,    14,    30,    22,    42,    43,    44,
      45,    46,    42,    43,    44,    45,    46,     2,    69,    64,
      65,    54,     9,    72,    35,     3,    66,    13,    10,    26,
      27,    45,    46,    33,    28,    29,     7,    14,    30,    42,
      43,    44,    45,    46,    14,    20,    21,    40,    47,    42,
      43,    44,    45,    46,    23,    48,    63,    68,    52,    42,
      43,    44,    45,    46,    67,    71,    24,    70
  };

  const unsigned char
  BisonParser::yycheck_[] =
  {
      13,    26,    18,    25,    29,    10,    11,    16,    21,    18,
       3,    16,    25,    18,    19,    40,     9,    42,    43,    44,
      45,    46,    16,    48,    18,    19,    19,     3,     4,     5,
       6,     7,     3,     4,     5,     6,     7,     0,    63,    12,
      13,    17,     9,    68,    66,     8,    17,    16,    15,    10,
      11,     6,     7,    66,    15,    16,    14,    18,    19,     3,
       4,     5,     6,     7,    18,    17,    13,    16,    12,     3,
       4,     5,     6,     7,    14,    16,     3,    13,    12,     3,
       4,     5,     6,     7,    17,    66,    21,    65
  };

  const unsigned char
  BisonParser::yystos_[] =
  {
       0,    21,     0,     8,    22,    18,    32,    14,    23,     9,
      15,    24,    32,    16,    18,    25,    26,    27,    31,    38,
      17,    13,    32,    14,    27,    28,    10,    11,    15,    16,
      19,    29,    30,    31,    35,    38,    39,    40,    30,    31,
      16,    30,     3,     4,     5,     6,     7,    12,    16,    32,
      36,    37,    12,    30,    17,    30,    30,    30,    30,    30,
      30,    33,    34,     3,    12,    13,    17,    17,    13,    30,
      37,    29,    30
  };

  const unsigned char
  BisonParser::yyr1_[] =
  {
       0,    20,    21,    21,    22,    23,    23,    24,    25,    25,
      26,    26,    27,    28,    28,    29,    29,    29,    29,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    31,    32,
      33,    33,    34,    34,    35,    36,    36,    37,    37,    38,
      39,    40
  };

  const unsigned char
  BisonParser::yyr2_[] =
  {
       0,     2,     0,     2,     5,     0,     2,     8,     0,     1,
       1,     3,     2,     0,     2,     2,     1,     1,     1,     3,
       3,     3,     3,     3,     4,     3,     1,     1,     1,     1,
       0,     1,     1,     3,     3,     1,     3,     1,     3,     1,
       3,     5
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BisonParser::yytname_[] =
  {
  "\"EOF\"", "error", "$undefined", "\"=\"", "\"-\"", "\"+\"", "\"*\"",
  "\"/\"", "\"class\"", "\"void\"", "\"return\"", "\"while\"", "\";\"",
  "\",\"", "\"{\"", "\"}\"", "\"(\"", "\")\"", "\"identifier\"",
  "\"number\"", "$accept", "class_list", "class", "method_list", "method",
  "param_list", "param_list_noemp", "decl_param", "stmt_list", "stmt",
  "expr", "name", "new_name", "arg_list", "arg_list_noemp", "decl_stmt",
  "decl_bean_list", "decl_bean", "type", "return_stmt", "while_stmt", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  BisonParser::yyrline_[] =
  {
       0,   102,   102,   103,   106,   110,   112,   116,   120,   122,
     125,   128,   132,   136,   138,   142,   144,   146,   148,   155,
     157,   159,   161,   163,   165,   167,   169,   171,   175,   179,
     183,   185,   189,   191,   195,   199,   202,   206,   208,   212,
     216,   219
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
#line 1444 "bison.cc" // lalr1.cc:1167
#line 222 "bison.yy" // lalr1.cc:1168


void yy::BisonParser::error(const location_type& l,
                              const std::string& m)
{
	throw ParseException(l, m);
}

