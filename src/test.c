/**
 * Copyright (c) 2012 ooxi/xml.c
 *     https://github.com/ooxi/xml.c
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software in a
 *     product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <xml.h>

typedef struct {
    uint8_t  name[32];
    unsigned int age;
	unsigned int number;
} STUDENT_st;

typedef struct {
    uint8_t  class[32];
    uint8_t  teacher[32];	
    unsigned int total;
    STUDENT_st student[3];
} STUDENT_LIST_st;

int getNodeInStr(struct xml_node* root, int index, uint8_t * str)
{
    struct xml_node* node = xml_node_child(root, index);

    if (node == NULL)
    {
        printf("not found\r\n");
        return -1;
    }
    struct xml_string* key   = xml_node_name(node);
    struct xml_string* value = xml_node_content(node);
    uint8_t* strkey          = calloc(xml_string_length(key) + 1, sizeof(uint8_t));
    uint8_t* strvalue        = calloc(xml_string_length(value) + 1, sizeof(uint8_t));

    xml_string_copy(key, strkey, xml_string_length(key));
    xml_string_copy(value, strvalue, xml_string_length(value));
    strcpy((char *)str, (char *)strvalue);
    //printf("%s %s\n", strkey, str);
    free(strkey);
    free(strvalue);

    return 0;
}

int getNodeInInt(struct xml_node* root, int index, unsigned int *pValue)
{
    struct xml_node* node = xml_node_child(root, index);

    if (node == NULL)
    {
        printf("not found\r\n");
        return -1;
    }
    struct xml_string* key   = xml_node_name(node);
    struct xml_string* value = xml_node_content(node);
    uint8_t* strkey          = calloc(xml_string_length(key) + 1, sizeof(uint8_t));
    uint8_t* strvalue        = calloc(xml_string_length(value) + 1, sizeof(uint8_t));

    xml_string_copy(key, strkey, xml_string_length(key));
    xml_string_copy(value, strvalue, xml_string_length(value));
    *pValue = atoi((const char*)strvalue);	
    //printf("%s %d\n", strkey, *pValue);
    free(strkey);
    free(strvalue);

    return 0;
}

int showStudentsInList(STUDENT_LIST_st *list)
{
    int i;

    if (list == NULL)
    {
        printf("null parameter\r\n");
        return -1;
    }

    printf("Class\t: %s \r\n", list->class);
    printf("Teacher\t: %s \r\n", list->teacher);	
    printf("Count\t: %d \r\n", list->total);
    for (i = 0; i < list->total; i++)
    {
        printf("==== Student [%02d] ==== \r\n", i);
        printf("Name\t: %s \r\n", list->student[i].name);
        printf("Age\t: %d \r\n", list->student[i].age);
        printf("Number\t: %d \r\n", list->student[i].number);
    }

    return 0;
}

int main(int argc, char** argv) {

	/* XML source, could be read from disk
	 */
	uint8_t source[] = ""
		"<LIST>"
			"<CLASS>1-1</CLASS>"
			"<TEACHER>Alice</TEACHER>"				
			"<TOTAL>3</TOTAL>"			
			"<ALLStudent>"
				"<Student>"
					"<NAME>Tom</NAME>"
					"<AGE>8</AGE>"
					"<NUMBER>1</NUMBER>"					
				"</Student>"
				"<Student>"
					"<NAME>Sally</NAME>"
					"<AGE>7</AGE>"
					"<NUMBER>2</NUMBER>"						
				"</Student>"
				"<Student>"
					"<NAME>Bill</NAME>"
					"<AGE>8</AGE>"
					"<NUMBER>3</NUMBER>"					
				"</Student>"				
			"</ALLStudent>"
		"</LIST>"
	;

	STUDENT_LIST_st list;
	int i;
	/* Parse the document
	 *
	 * Watch out: Remember not to free the source until you have freed the
	 *     document itself. If you have to free the source before, supply a
	 *     copy to xml_parse_document which can be freed together with the
	 *     document (`free_buffer' argument to `xml_document_free')
	 */
	struct xml_document* document = xml_parse_document(source, strlen((char *)source));

	/* You _have_ to check the result of `xml_parse_document', if it's 0
	 * then the source could not be parsed. If you think this is a bug in
	 * xml.c, than use a debug build (cmake -DCMAKE_BUILD_TYPE=Debug) which
	 * will verbosely tell you about the parsing process
	 */
	if (!document) {
		printf("Could parse document\n");
		exit(EXIT_FAILURE);
	}
	struct xml_node* root = xml_document_root(document);
	getNodeInStr(root, 0, list.class);		//class
	getNodeInStr(root, 1, list.teacher);	//teacher
	getNodeInInt(root, 2, &list.total);		//total count
	
	struct xml_node* allStudentNode = xml_node_child(root, 3);	//ALLStudent
	struct xml_node* StudentNode;
	for(i = 0; i< list.total;i++)
	{
		StudentNode = xml_node_child(allStudentNode, i);	//Student
		getNodeInStr(StudentNode, 0, list.student[i].name);	//name
		getNodeInInt(StudentNode, 1, &list.student[i].age);	//age
		getNodeInInt(StudentNode, 2, &list.student[i].number);	//number
	}

	showStudentsInList(&list);
	/* Remember to free the document or you'll risk a memory leak
	 */
	xml_document_free(document, false);
	
	return 0;
}

