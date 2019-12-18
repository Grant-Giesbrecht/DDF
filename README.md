# KV-Spec
## What it is:
A specification defining the KV data file format. KV files prioritize clarity and documentation of the file's data above all else.
  * Plain text is used for easy editing and reading, however all data inside the file must be named and can have optional comments.
  * Additionally, the format supports a header string for the entire file, making it extremely easy to label your data.  
I designed this format with research in mind. Headers make it trivial to record the conditions under which an experiment was conducted, and variable names and comments give the user flexibility in how to document data with units and descriptions. Note that the current iteration of KV files are *not* ideal for colossal data sets. As plain text files, storage size is sacrificed for ease of editing.

Note: A second type of file, KV2 files, which are compressed and optionally encrypted for storing large data files or sensitive data may later be added. For the remainder of this documentation I will refer to KV1 files, the plain-text incarnation of KV, simply as KV files as they are the only type presently available.

Note: When describing syntax, parenthesis will indicate optional components, and angled brackets will indicate parameter names that must be replaced with their actual values. Literal angle brackets will be escaped with a backslash.

## Using KV Files in your Programs:
You have multiple options for integrating KV support into your programs.

## How it Works - Internal Structure:
Every KV file consists of these elements:
  * Version statements
  * Header statements
  * Inline comments
  * Inline variable statements
  * Vertical blocks
  * Vertical variable statements

### Version statement:
The first line of every KV file must be a version statement. Its syntax is: `#VERSION <major>.<minor>`. Ex: `#VERSION 2.0`.
### Header statement:
This element is optional, but must come before any variable statements. It allows the author to add a text description of the file's data. A header statement is started and ended with `#HEADER`. Any content between the lines containing `#HEADER` will be included in the header. Header text can not be on the same line as the `#HEADER` start/end statement.
### Inline comments:
This element behaves exactly the same way a C comment does. Start the inline comment with two forward slashes. Everything else on the line will be ignored. Multi-line comments are not
implimented.
### Inline variable statements:
Variable statements are the primary means of storing data in KV files. Inline variable statements define a single variable on a single line. They follow the basic format of `<type> <name> <data> (;)(?<variable-description>)`. The ending semicolon is optional, as is variable description. All characters after the question mark are included in the variable description, except for trailing whitespace, which is trimmed. Note that matrices or matrices (eg. `m<m<d>>`) are *not* supported or required. A 2D matrix uses the same declaration as a 1D matrix, however 3D matrices are not yet supported.

  * #### Data types:
  The available data types in KV files are double precision floats, strings, booleans, and matrices of any one data type. Type is indicated with a single letter. Matrices accept the data type in angle brackets. Ex
  * #### Data values:
    * Booleans:
    The full words `true` and `false`, case in-sensitive.
	* Floats: Takes any decimal number. Understands scientific notation indicated with 'e' or 'E'.
	* Strings: Standard C++ strings. Must be enclosed with double quotes. Escape double quotes with a backslash.
	* Matrices: Enclose data elements with square brackets. Separate values with comments. Separate lines with semicolons. Whitespace between elements is suggested for readability but not required.

Here are a few example variable statements:
  * `d pi 3.141592535;`
  * `s name "Tyr" ?This is my name`
  * `b online true`
  * `m<d> abcd [14, 7;3, 2]`

### Vertical blocks:
Vertical blocks indicate a region in which variable statements are vertical rather than inline. Inline variable statements cannot occur inside vertical blocks and vertical variable statements cannot occur outside of vertical blocks. Vertical blocks, similarly to header statements, are enclosed by lines containing a single string, `#VERTICAL`.

### Vertical variable statements:
Vertical variable statements follow the same general format as inline variable statements, with a few notable exceptions:
  * As the name implies, the components are arranged vertically
  * Multiple variables' elements can be and typically are present on one line, separated by spaces and/or tabs.
  * only matrices are supported
  * Semicolons are not used at the end of lines
  * Matrices:
    * are not surrounded by square brackets
	* Do not use commas to separate elements
	* semicolons still separate rows in a 2D matrix
  * Descriptions are still optional, but instead of appearing at the end of a line, they must appear below the variable names. Descriptions for multiple variables are separated by question marks. If one variable in the vertical block has a description, all must have descriptions, although they can be left blank by not putting text after the question mark.
  * Only one vertical variable statement may be present in each vertical block. However, the the
  single variable statement may define multiple variables (as shown in example below)

Syntax format:
  * Line 1: `<type-var-1> <type-var-2> ... <type-var-n>` Declare types.
  * Line 2: `<name-var-1> <name-var-2> ... <name-var-n>` Specify names.
  * Line 3: `(?<desc-var-1> ?<desc-var-2> ... ?<desc-var-n>)` Optionally provide descriptions for each variable.
  * Line 4: `<val1-var-1> <val1-var-2> ... <val1-var-n>` First element
  * Line 5: `<val2-var-1> <val2-var-2> ... <val2-var-n>` Second element
  * ...
  * Line m: `<val-var-1> <valm-var-2> ... <valm-var-n>` Last element

Here's an example vertical block:

    #VERTICAL
	m<d> m<d> m<b> m<s>
	Vin  Vout amp_on notes
	?Input voltage (V) ?Output voltage (V) ?Amplifier state ?Measurement
	1	 .99  false	 ""
	//Comments are still a-okay
	2	 2.01 false  ""
	3    5.87 true   "a = +6dB" //Comments after lines are also fine.
	4    8.1  true   "" //Note the pretty column arrangement is only for the viewer's benefit. To KV readers, whitespace is whitespace
	#VERTICAL
