#include <string>
#include <list>

class q_string_element;
typedef std::list<q_string_element> q_string;
class q_string_list;
class q_string_list_element;

enum q_string_element_type {
    QS_COPY,
    QS_REFERENCE
};

class q_string_element
{
public:
    q_string_element_type type;
    std::string data;
    
    q_string_element(q_string_element_type type, std::string &data);
};

class q_string_list
{
public:
    std::string name;
    std::list<q_string_list_element> elements;
    
    q_string_list(std::string &name);
};

class q_string_list_element
{
public:
    long weight;
    q_string data;
    
    q_string_list_element(long weight);
};

void AppendTempStringToGlobalString( void );
void AllocateGlobalStringList( std::string &name );
void AppendCopyStringToGlobalTempString( std::string &data );
void AppendRefStringToGlobalTempString( std::string &ref );
void AllocateGlobalStringListElement( long weight );
void AppendTempStringToGlobalStringList( void );

void RenderText( void );
void DumpState( void );
