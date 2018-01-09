#include "tao/pegtl.hpp"
#include <fstream>

namespace classify {
namespace grammar {
    using namespace tao::pegtl;
    using namespace tao::pegtl::ascii;

struct kw_end : not_at<identifier_other> { };

template <typename kw>
struct keyword : seq<kw, kw_end> { };

struct a     : keyword<one<'a'>> { };
struct an    : keyword<TAOCPP_PEGTL_ISTRING("an")> { };
struct the   : keyword<TAOCPP_PEGTL_ISTRING("the")> { };
struct is    : keyword<TAOCPP_PEGTL_ISTRING("is")> { };
struct why   : keyword<TAOCPP_PEGTL_ISTRING("why")> { };
struct NOT   : keyword<TAOCPP_PEGTL_ISTRING("not")> { };
struct bye   : keyword<TAOCPP_PEGTL_ISTRING("bye")> { };
struct dump  : keyword<TAOCPP_PEGTL_ISTRING("dump")> { };
struct save  : keyword<TAOCPP_PEGTL_ISTRING("save")> { };
struct load  : keyword<TAOCPP_PEGTL_ISTRING("load")> { };
struct help  : keyword<TAOCPP_PEGTL_ISTRING("help")> { };
struct forget: keyword<TAOCPP_PEGTL_ISTRING("forget")> { };
struct QMARK : one<'?'> { };

struct article : sor<the, an, a> { };

struct article1 : article { };
struct article2 : article { };

struct thing1 : identifier { };
struct thing2 : identifier { };
struct filename : plus<print> { };

template <typename Rule>
struct padr : seq<Rule, star<space>> { };

struct assertion : seq<
                     padr<article1>,
                     padr<thing1>,
                     padr<is>,
                     opt<padr<NOT>>,
                     padr<article2>,
                     padr<thing2>
                   > { };

struct query : seq<
                 padr<is>,
                 padr<article1>,
                 padr<thing1>,
                 opt<padr<NOT>>,
                 padr<article2>,
                 padr<thing2>,
                 padr<QMARK>
               > { };

struct explain : seq<
                   padr<why>,
                   padr<is>,
                   padr<article1>,
                   padr<thing1>,
                   opt<padr<NOT>>,
                   padr<article2>,
                   padr<thing2>,
                   padr<QMARK>
                 > { };

struct explain_last : seq<
                        padr<why>,
                        padr<QMARK>
                      > { };

struct saveCmd : seq< padr<save>, padr<filename> > { };
struct loadCmd : seq< padr<load>, padr<filename> > { };

struct command : sor<
                   assertion,
                   query,
                   explain,
                   explain_last,
                   padr<bye>,
                   padr<dump>,
                   padr<saveCmd>,
                   padr<loadCmd>,
                   padr<help>,
                   padr<forget>
                 > { };

struct grammar : seq<command, eof> { };


} // namespace grammar

struct state
{
    state() : _is(true), _again(true)  {  }
    std::string _article1;
    std::string _thing1;
    std::string _article2;
    std::string _thing2;
    bool        _is;
    bool        _again;
};

Explainer explainer;

template <typename Rule> struct action : tao::pegtl::nothing<Rule> { };

template<> struct action<grammar::bye>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        std::cout << "Goodbye." << std::endl;
        s._again = false;
    }
};

template<> struct action<grammar::article1>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        s._article1 = in.string();
    }
};

template<> struct action<grammar::article2>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        s._article2 = in.string();
    }
};

template<> struct action<grammar::thing1>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        s._thing1 = in.string();
    }
};

template<> struct action<grammar::thing2>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        s._thing2 = in.string();
    }
};

template<> struct action<grammar::filename>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        s._thing1 = in.string();
    }
};

template<> struct action<grammar::NOT>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        s._is = false;
    }
};

template<> struct action<grammar::assertion>
{
    template<typename Input>
    static void apply(const Input &in, state &s)
    {
        auto thing1 = Thing::fromString(s._article1, s._thing1);
        auto thing2 = Thing::fromString(s._article2, s._thing2);

        explainer.reset();
        if (thing1 == thing2) {
            if (s._is) {
                std::cout << "That is self-evident!" << std::endl;
            } else {
                std::cout << "That is a contradiction!" << std::endl;
            }
            explainer.addFact(thing1, thing1, s._is);
            return;
        }
        bool is;
        if (thing1->isA(thing2, explainer, is)) {
            if (is == s._is) {
                std::cout << "I already knew that." << std::endl;
                return;
            }
        }
        thing1->addIs(thing2, s._is);
        std::cout << "OK" << std::endl;
    }
};

template<> struct action<grammar::query>
{
    template<typename Input>
    static void apply(const Input &in, state &s)
    {
        auto thing1 = Thing::fromString(s._article1, s._thing1);
        auto thing2 = Thing::fromString(s._article2, s._thing2);

        explainer.reset();
        if (thing1 == thing2) {
            if (s._is) {
                std::cout << "Yes, of course!" << std::endl;
            } else {
                std::cout << "How can that be?" << std::endl;
            }
            explainer.addFact(thing1, thing1, s._is);
            return;
        }
        auto is = true;
        if (thing1->isA(thing2, explainer, is)) {
          is = !(is ^s._is);
          std::cout << (is ? "Yes." : "No.") << std::endl;
        } else {
            std::cout << "Not that I'm aware of." << std::endl;
        }
    }
};

template<> struct action<grammar::explain>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        auto thing1 = Thing::fromString(s._article1, s._thing1);
        auto thing2 = Thing::fromString(s._article2, s._thing2);

        explainer.reset();
        if (thing1 == thing2) {
            if (s._is) {
                std::cout << "Because that is a tautology." << std::endl;
            } else {
                std::cout << "Because that would be a contradiction." << std::endl;
            }
            explainer.addFact(thing1, thing1, s._is);
            return;
        }
        bool is;
        auto b = thing1->isA(thing2, explainer, is);
        if (b && (is != s._is)) {
            if (is) {
                std::cout << "But, it is..." << std::endl;
            } else {
                std::cout << "But, it isn't..." << std::endl;
            }
        }
        explainer.explain();
    }
};

template<> struct action<grammar::explain_last>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        explainer.explain();
    }
};

template<> struct action<grammar::dump>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        std::cout << "These are the facts I know:" << std::endl;
        Thing::save(std::cout);
    }
};

template<> struct action<grammar::saveCmd>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        std::ofstream file;
        file.open(s._thing1.c_str());
        if (!file) {
            std::cout << "Cannot open: " << s._thing1 << ": "
                      << strerror(errno) << std::endl;
            return;
        }
        Thing::save(file);
        file.close();
    }
};

template<> struct action<grammar::loadCmd>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        std::ifstream file;
        file.open(s._thing1.c_str());
        if (!file) {
            std::cout << "Cannot open: " << s._thing1 << ": "
                      << strerror(errno) << std::endl;
            return;
        }
        Thing::load(file);
        file.close();
    }
};

void help(void)
{
    using std::cout;
    using std::endl;

    cout << "Welcome to classify -- a demonstration of AI" << endl;
    cout << "I understand the following types of sentences: " << endl << endl;
    cout << "  Assertions of the form: <article> <noun> is [not] <article> <noun>" << endl;
    cout << "          for example:    a carrot is a tuber" << endl;
    cout << "                          a carrot is not an animal" << endl;
    cout << "  Queries of the form:    is <article> <noun> [not] <article> <noun>?" << endl;
    cout << "                   or:    why is <article> <noun> [not] <article> <noun>?" << endl;
    cout << "           or, simply:    why?" << endl;
    cout << "          for example:    is a carrot a tuber?" << endl;
    cout << "                          is a carrot not an animal?" << endl;
    cout << "                          why is a carrot a tuber?" << endl;
    cout << "                          why?" << endl;
    cout << "  (please note the question mark is necessary.)" << endl << endl;
    cout << "  Commands of the form:   save <filename>" << endl;
    cout << "                          load <filename>" << endl;
    cout << "                          bye" << endl;
    cout << "                          forget" << endl;
    cout << "                          help" << endl << endl;
}


template<> struct action<grammar::help>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        help();
    }
};

template<> struct action<grammar::forget>
{
    template <typename Input>
    static void apply(const Input &in, state &s)
    {
        Thing::forget();
        std::cout << "I've forgotten everything you told me." << std::endl;
    }
};

} // namespace classify

