#include "qstring.h"
#include <iostream>
#include <random>
#include <stdlib.h>
#include <cstdio>
#include <cstring>

q_string *g_root_string;
q_string_list *g_substitution_list;

std::random_device generator;

q_string::q_string(char *data):
data(data),
next(NULL){}

q_string_copy::q_string_copy(char *data):
q_string(data){}

void q_string_copy::render(void){
    std::printf("%s",data);
    if(next){
        next->render();
    }
}

void q_string_copy::verify(void){
    if(next)
        next->verify();
}

q_string_ref::q_string_ref(char *data, parse_state *ps, int first_line, int first_column):
q_string(data),
ps(ps),
first_line(first_line),
first_column(first_column){}

void q_string_ref::render(void){
    find_and_render();
    if(next) next->render();
}

void q_string_ref::verify(void){
    q_string_list *list = g_substitution_list;
    if(!list){
        printf("No substitution lists\n");
        exit(EXIT_FAILURE);
    }
    bool found = false;
    while(list){
        if(strcmp(list->id,data)==0){
            found = true;
            break;
        }
        list = list->next;
    }
    if(!found){
        printf("Reference not found |%s| in file \"%s\" [line:%d,column:%d]\n",
               data,
               ps->filename,
               first_line, first_column
              );
        exit(EXIT_FAILURE);
    }
    if(next) next->verify();
}

void q_string_ref::find_and_render(void){
    q_string_list *list = g_substitution_list;
    if(!list){
        printf("No substitution lists!\n");
        return;
    }
    do
    {
        if(strcmp(list->id,data)==0){
            // found the list
            // find the total weight
            long total_weight = 0;
            q_string_list_element *element = list->elements;
            while(element){
                total_weight += element->weight;
                element = element->next;
            }
            
            // pick a random number
            std::uniform_int_distribution<int> distribution(0,total_weight-1);
            long x_pick = distribution(generator);

            // select the element
            long x_base = 0;
            element = list->elements;
            do {
                if(x_pick>=x_base && x_pick<(x_base+element->weight)){
                    // render it
                    element->str->render();
                    return;
                }
                x_base += element->weight;
                element = element->next;
            } while(element);
            return;
        }
        list = list->next;
    }while(list);
    printf("reference id not found for \"%s\"\n",data);
    return;
}

q_string_list::q_string_list(char *id, q_string_list_element *elements):
id(id),
next(NULL),
elements(elements){}

q_string_list_element::q_string_list_element(long weight, q_string *str):
weight(weight),
str(str),
next(NULL){}

void RenderText( void )
{
    g_root_string->render();
}

void VerifyText( void )
{
    g_root_string->verify();
    
    q_string_list *list = g_substitution_list;
    while(list){
        q_string_list_element *element = list->elements;
        while(element){
            element->str->verify();
            element = element->next;
        }
        list = list->next;
    }
}
