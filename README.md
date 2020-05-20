# funny-text

A recursive and random text substitution program. With the appropriate intput file the resulting text can be funny. Especially when rendered with a text-to-speech application.

Also demonstrates the use of flex and bison to create the lexical analyzer and parser.

For an example of the grammar see one of the .rtxt files.

To make 'rtext' simply type make.

To make 'rtext' you will need g++, make, flex and bison.

To run 'rtext' on the command prompt type:

$./rtext example.rtxt

or

$ cat example.rtxt | ./rtext
