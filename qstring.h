#include <cstddef>
#include "parser.h"

class q_string
{
public:
    char *data;
    q_string *next;
public:
    q_string(char *data);
    virtual void render(void)=0;
    virtual void verify(void)=0;
};

class q_string_copy : public q_string
{
public:
    q_string_copy(char *data);
    void render(void);
    void verify(void);
};

class q_string_ref : public q_string
{
private:
    parse_state *ps;
    int first_line;
    int first_column;
public:
    q_string_ref(char *data, parse_state *ps,
        int first_line, int first_column );
    void render(void);
    void verify(void);
    void find_and_render(void);
};

class q_string_list_element;

class q_string_list
{
public:
    char *id;
    q_string_list *next;
    q_string_list_element *elements;
    q_string_list(char *id, q_string_list_element *elements);
};

class q_string_list_element
{
public:
    long weight;
    q_string *str;
    q_string_list_element *next;
    q_string_list_element(long weight, q_string *str);
};

template <class X> void splice(X **head, X *tail){
    if(*head==NULL){
       *head = tail;
    }else{
        X *x=*head;
        while(x->next){
            x = x->next;
        }
        x->next = tail;
    }
}

extern q_string *g_root_string;
extern q_string_list *g_substitution_list;

void VerifyText( void );
void RenderText( void );
