# funny-text

A recursive and random text substitution program. With the appropriate intput file the resulting text can be funny. Especially when rendered with a text-to-speech application.

Also demonstrates the use of flex and bison to create the lexical analyzer and parser.

For an example of the grammar see one of the .rtxt files. The grammer consists of strings, substitution lists and includes.

Strings are delimited with '<' and '>', for example:

    <This is a string.>

A string can have references to a substitution list. The subsititution reference is enclosed with the character '|', for example:

    <This is a string with a |reference_id|.>

A substitution list is a list of strings. Each string in the list also has a integer weight. The higher the weight the more likely it is to be selected. The syntax for a substitution list is as follows:

    [ref_id]:
    [weight 0] string0
    [weight 1] string1
    ...
    ;

For example, the 'reference_id' of the previous string example could have a substitution list like this:

    reference_id:
    1 <substitution(this gets substituted 1/6th of the time)>
    2 <replacement(this gets substituted 1/3rd of the time)>
    3 <alternative(this gets substituted 1 half of the time)>
    ;

To include another file delimit it in '|', for example:

    |include_this_file.rtxt|
    
Notice that it is the exact same syntax as a reference to a substitution list. If it's outside of a string then it's considered to be an include directive.
    
    
    
To make 'rtext' simply type make.

To make 'rtext' you will need g++, make, flex 2.6.4 and bison 3.6 .

To run 'rtext' on the command prompt type:

    $./rtext example.rtxt

or

    $cat example.rtxt | ./rtext
