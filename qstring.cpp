#include "qstring.h"
#include <iostream>
#include <random>
#include <stdlib.h>

q_string_element::q_string_element(
    q_string_element_type type,
    std::string &data)
{
    q_string_element::type = type;
    q_string_element::data = data;
}

q_string_list::q_string_list( std::string &name ){
    q_string_list::name = name;
}

q_string_list_element::q_string_list_element(long weight){
    q_string_list_element::weight = weight;
}

q_string g_temp_string;
q_string g_root_string;
std::list<q_string_list> g_substitution_list;
std::random_device generator;

void AppendTempStringToGlobalString( void )
{
    g_root_string.splice( g_root_string.end(), g_temp_string );
}

void AllocateGlobalStringList( std::string &name )
{
     g_substitution_list.emplace_back( name );
}

void AppendCopyStringToGlobalTempString( std::string &data )
{
    g_temp_string.emplace_back( QS_COPY, data );
}

void AppendRefStringToGlobalTempString( std::string &data )
{
    g_temp_string.emplace_back( QS_REFERENCE, data );
}

void AllocateGlobalStringListElement( long weight )
{
    g_substitution_list.back().elements.emplace_back( weight );
}

void AppendTempStringToGlobalStringList( void )
{
    g_substitution_list.back().elements.back().data.splice( g_substitution_list.back().elements.back().data.end(), g_temp_string );
}

void RenderString( q_string &str);

void RenderReference(std::string &ref){
    // find this reference in the substitution list
    for(auto &list:g_substitution_list){
        if(list.name.compare(ref)==0){
            // found it
            // calculate the total weight
            long total_weight = 0;
            for(auto &element:list.elements){
                total_weight += element.weight;
            }
            // pick a random number
            std::uniform_int_distribution<int> distribution(0,total_weight-1);
            long x_pick = distribution(generator);
            // find the element that 
            long x_base = 0;
            for(auto &element:list.elements){
                if(x_pick>=x_base && x_pick<(x_base+element.weight)){
                    RenderString( element.data );
                    return;
                }
                x_base += element.weight;
            }
        }
    }
    std::cout << "Couldn't find reference:" << ref << std::endl;
}

void RenderString( q_string &str )
{
    for(auto &element:str){
        switch(element.type){
            case QS_COPY:
                std::cout << element.data;
                break;
            case QS_REFERENCE:
                RenderReference(element.data);
                break;
        }
    }
}

void RenderText( void )
{
    RenderString( g_root_string );
    std::cout << std::endl;
}

void DumpString( q_string str )
{
    for(auto &element:str){
        switch(element.type){
            case QS_COPY:
                std::cout << element.data;
                break;
            case QS_REFERENCE:
                std::cout << "|" << element.data;
                break;
        }
    }
}


void DumpState( void )
{
    // print the contents of the root string
    
    // print the contents of the substitution list
    std::cout << "Substitution list" << std::endl;
    for(auto &list:g_substitution_list){
        std::cout << "Name:" << list.name << std::endl;
        for(auto &element:list.elements){
            std::cout << "\tweight:" << element.weight;
            std::cout << " data:";
            DumpString(element.data);
            std::cout << std::endl;
        }
    }
}







