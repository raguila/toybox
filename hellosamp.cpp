#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <ctype.h>
#include <vector>
#include <map>

/*
PL: ToyBox
Authors: Roven Rommel Fuentes
	 Roinand Aguila
 	 Mary Grace Angelie Aguirre
	 Diana Magbanua
Status: Creation of parse tree
Last Edited: April 15, 2015
Should function be inside or outside OPEN/CLOSEBOX?
*/

#define MAXWORD 60 //maximum word size

using namespace std;

typedef enum{
    tokRDUCKY,tokNUMBLK,tokLETBLK,tokJROPE,tokMBOX,tokRACK,//datatype
    tokOPPLAY,tokCLPLAY,tokOPBOX,tokCLBOX,//functions
    tokADD,tokSUB,tokMULT,tokDIV,tokMOD, //arithmetic operators
    tokSMLR,tokBGGR,tokASMA,tokABIA,tokTSA,tokNTSA,//relational operators
    tokAND,tokOR,tokNOT,//logical operators
    tokUP,tokDWN,tokTIE,tokRULER,//other operators
    tokLPAR,tokRPAR,tokCOMMA,tokLBRAC,tokRBRAC,tokQUOTE,tokHASH,tokCOLON,tokEXCLA,//delimeter
    tokBOARD,tokHOLE,tokHOOLA,tokHOOP,tokFIND,tokVAL,//other keywords
    tokRETURN,tokSHOW,tokCOLLECT,tokSTOP,tokJUMP,tokEXPUNC,
    tokAS,//assignment
    tokIDEN, //identifier
    NAtk,
    tokEND 

}TokenType;

typedef struct tokenTag{
    //string str; 
    char str[MAXWORD];
    TokenType type; 
    int pos; //line number
}Token;

typedef enum{
    boxNode,decNode,idenNode,idenmorNode,valNode,hoolaNode,boardNode,conditionNode,playNode,
    showNode,outstrNode,collectNode,asgnNode,stmtNode,stmtmorNode,exprNode,op1Node,op2Node,
    op3Node,relOpNode
}NodeType;

typedef struct nodeTag{
    NodeType type;
    Token *tokenptr;
    struct nodeTag *child1;
    struct nodeTag *child2;
    struct nodeTag *child3;
    struct nodeTag *child4;
    struct nodeTag *child5;
}Node;

int myargc;
char *myargv[500];
char arithOperator[]={'*','/','+','-','%'};
char delimeter[]={'(',')',',','[','"','#',':','=',};
map<string,TokenType> reservedWord;
string code;
int offset=0,lineNum=0,tokcount=0;
Token *toks=NULL;
Token tk = {"N/A",NAtk,0};

//FUNCTIONS
void make_tokenmap();
int isReservedChar(char c);
int isDelimeter(char c);
TokenType getDelimeterType(char c);
int isArithOperator(char c);
TokenType getArithType(char c);
int isDatatype(TokenType type);
int isStmt(TokenType type);
Token anlzr();
int scan_code(string linestream,int &lineNum);
Node* createNode(NodeType type);
Node* value();
Node* iden_more();
Node* iden();
Node* declartn();
Node* op1();
Node* op2();
Node* op3();
Node* loop();
Node* expr();
Node* board();
Node* condition();
Node* showout();
Node* outstring();
Node* stmtset();
Node* box();
void parser();

void make_tokenmap(){
    //datatype
    reservedWord["RDUCKY"]=tokRDUCKY;
    reservedWord["NUMBLK"]=tokNUMBLK;
    reservedWord["LETBLK"]=tokLETBLK;
    reservedWord["JROPE"]=tokJROPE;
    reservedWord["MBOX"]=tokMBOX;
    reservedWord["RACK"]=tokRACK;
    //functions
    reservedWord["OPENBOX"]=tokOPBOX;
    reservedWord["CLOSEBOX"]=tokCLBOX;
    reservedWord["OPEN_PLAYPEN"]=tokOPPLAY;
    reservedWord["CLOSE_PLAYPEN"]=tokCLPLAY;
    //relational operator
    reservedWord["SMALLER_THAN"]=tokSMLR;
    reservedWord["BIGGER_THAN"]=tokBGGR;
    reservedWord["AS_SMALL_AS"]=tokASMA;
    reservedWord["AS_BIG_AS"]=tokABIA;
    reservedWord["THE_SAME_AS"]=tokTSA;
    reservedWord["NOT_THE_SAME_AS"]=tokNTSA;
    //logical operator
    reservedWord["AND"]=tokAND;
    reservedWord["OR"]=tokOR;
    reservedWord["NOT"]=tokNOT;
    //others
    reservedWord["LEVEL_UP"]=tokUP;
    reservedWord["LEVEL_DOWN"]=tokDWN;
    reservedWord["TIE_WITH"]=tokTIE;
    reservedWord["RULER"]=tokRULER;
    reservedWord["MATCHBOARD"]=tokBOARD;
    reservedWord["HOLE"]=tokHOLE;
    reservedWord["HOOLA"]=tokHOOLA;
    reservedWord["HOOP"]=tokHOOP;
    reservedWord["FIND_PLAYMATE"]=tokFIND;
    reservedWord["RETURN_TO_PLAYMATE"]=tokRETURN;
    reservedWord["SHOW"]=tokSHOW;
    reservedWord["COLLECT"]=tokCOLLECT;
    reservedWord["AS"]=tokAS;
    reservedWord["STOP"]=tokSTOP;
    reservedWord["JUMP"]=tokJUMP;
}

int isReservedChar(char c){
    if (c=='(' || c==')' || c==',' || c=='[' || c==']' || c=='!' || c=='"' || c=='#' ||
	c==':' || c=='+' || c=='-' || c=='*' || c=='/' || c=='%' || c=='=' || c=='<' || 
	c=='>'){
	return 1;
    }else return 0;
}

int isDelimeter(char c){
    if (c=='(' || c==')' || c==',' || c=='[' || c==']' || c=='"' || c=='#' || c==':' || c=='!' ){
	return 1;
    }else return 0;
}

TokenType getDelimeterType(char c){
    if(c=='(') return tokLPAR;
    if(c==')') return tokRPAR;
    if(c==',') return tokCOMMA;
    if(c=='[') return tokLBRAC;
    if(c==']') return tokRBRAC;
    if(c=='"') return tokQUOTE;
    if(c=='#') return tokHASH;
    if(c==':') return tokCOLON;
    if(c=='!') return tokEXCLA;
}

int isArithOperator(char c){
    if (c=='+' || c=='-' || c=='*' || c=='/' || c=='%')
	return 1;
    else return 0;
}

TokenType getArithType(char c){
    if(c=='+') return tokADD;
    if(c=='-') return tokSUB;
    if(c=='*') return tokMULT;
    if(c=='/') return tokDIV;
    if(c=='%') return tokMOD;
}

int isDatatype(TokenType type){
   if(type==tokRDUCKY || type==tokNUMBLK || type==tokLETBLK || type==tokJROPE ||
      type==tokMBOX || type==tokRACK)
	return 1;
   else return 0;
}

int isStmt(TokenType type){
   if(type==tokOPPLAY || type==tokBOARD || type==tokHOOLA || type==tokSHOW || type==tokCOLLECT || type==tokIDEN)
	return 1;
   else return 0;
}

int isRelOp(TokenType type){
   if(type==tokSMLR || type==tokBGGR || type==tokASMA || type==tokABIA || type==tokTSA || type==tokNTSA)
	return 1;
   else
	return 0;
}

Token anlzr(){
    int idx=0;
    Token token; 
    string temp; 
    for(int i=offset;i<code.size();i++){
	if(code[i]=='\n'){
	    lineNum++;
	    continue; 
	}

	if(code[i]=='-'){
	    if(code[i+1]=='-'){ //1-line comment
		while(code[i]!='\n'){i++;}
		lineNum++; 
		continue;
		return token;
	    }
	}else if(code[i]=='<'){
	    if(code[i+1]=='!'){ //multi-line comment
		while(!(code[i]=='!' && code[i+1]=='>')){
		    if(code[i]=='\n') lineNum++;
		    i++;
		} i++;
		continue;
 	    }
	}

	if(isalpha(code[i])){ //save keywords/identifiers
	    idx=i;
	    while(isalnum(code[i]) || code[i]=='_'){i++;}
	    temp=code.substr(idx,i-idx).c_str();
            strcpy(token.str,temp.c_str());
	    token.pos = lineNum;
            if(reservedWord.find(temp)!=reservedWord.end()){ 
		token.type = reservedWord.find(temp)->second;
	    }else{
		token.type = tokIDEN;
	    }
	    //printf("%s\n",temp.c_str());
	    offset=i; 
	    return token;
	}else if(isdigit(code[i])){
	    idx=i;
  	    while(isdigit(code[++i])){}
	    temp=code.substr(idx,i-idx).c_str();
	    strcpy(token.str, temp.c_str());
	    token.pos = lineNum;
	    token.type = tokVAL;
	    //printf("%s\n",temp.c_str());
	    offset = i--;
	    return token;
	}else if(ispunct(code[i])){
    	    token.str[0]=code[i];
    	    token.str[1]='\0'; 
	    if(isDelimeter(code[i])){
    		token.pos = lineNum;
    		token.type = getDelimeterType(code[i]);
	    }else if(isArithOperator(code[i])){
    		token.pos = lineNum; 
    		token.type = getArithType(code[i]); 
	    }else{ 
    		token.pos = lineNum;
    		token.type = tokEXPUNC;
	    }
	    //printf("%s\n",token.str);
	    offset = ++i;
	    return token;
	}
    }
    token.type = tokEND;
    return token;
}

int scan_code(string linestream,int &lineNum){ //check for invalid characters
    int wordsize=0;
    char word[MAXWORD];
    for(int i=0;i<linestream.size();i++){
	if(linestream[i]=='\n'){ 
	    lineNum++;
	    continue;
	}

	if(linestream[i]=='-'){ 
	    if(linestream[i+1]=='-'){ //ignore comments
		lineNum++;
		continue;
	    }
	}

    	if(isalnum(linestream[i])){
	    word[wordsize++]=linestream[i];
	    if(wordsize==MAXWORD){
		  printf("ERROR: A word in line %d is too long. \n",lineNum);
		  return 1;
	    }
	}else if(isspace(linestream[i]) || isReservedChar(linestream[i])){
	    wordsize = 0;
	}else if(ispunct(linestream[i])){
	    continue;
	}else{
	    printf("Error: Invalid character '%c' in line %d. \n",linestream[i],lineNum);
	    return 1;
	}

	
    }
}
Node* createNode(NodeType type){
    Node *node = (Node*) malloc(sizeof(Node));
    node->type = type;
    node->tokenptr = NULL;
    node->child1 =node->child2 =node->child3 =node->child4 =NULL;
    return node;
}

Node* value(){
    Node *node = createNode(valNode);
    if(tk.type==tokVAL){
	printf("AS %s",tk.str);
	tk = toks[offset++]; 
    }else{
	printf("ERROR: %s is not the expected value in line %d.\n",tk.str,tk.pos);
   	exit(1);
    }
    //node->tokenPtr=tk;
    return node;
}

Node* iden_more(){
    Node *node = createNode(idenmorNode);
    if(tk.type==tokCOMMA){
	tk = toks[offset++];
	if(tk.type==tokIDEN){  //Variable Identifier
     	    //TO DO:insert Var to list
	    //node->tokenPtr = tk;
	    printf(",%s ",tk.str);
	    tk = toks[offset++]; 
	}else{
	    printf("ERROR: An identifier is expected in line %d.\n",tk.pos);
	    exit(1);
	}

        if(tk.type==tokAS){ //Value Assignment
	    tk = toks[offset++];
	    node->child1 = value();
	    //TO DO: assign value to variable
	}
	node->child2 = iden_more();
    }else{
	return node;
    } 
}

Node* iden(){
   Node *node = createNode(idenNode);
   if(isDatatype(tk.type)){  //check Variable Type
	//TO DO: later divide to dif types
	printf("%s ",tk.str);
	tk = toks[offset++]; 
	if(tk.type==tokIDEN){  //Variable Identifier
     	    //TO DO:insert Var to list
	    //node->tokenPtr = tk;
	    printf("%s ",tk.str);
	    tk = toks[offset++]; 
	}else{
	    printf("ERROR: An identifier is expected in line %d.\n",tk.pos);
	    exit(1);
	}

        if(tk.type==tokAS){ //Value Assignment
	    tk = toks[offset++];
	    node->child1 = value();
	    //TO DO: assign value to variable
	}
	node->child2 = iden_more();
        if(tk.type==tokHASH){ 
	    printf("#\n");
	    tk = toks[offset++]; 
	}else{
	    printf("ERROR: Line %d is not ended properly.\n",tk.pos);
	    exit(1);
	}
	return node; 
    }else{
	    return node; //declaration may be empty
    }
}

Node* declartn(){
    Node *node = createNode(decNode);
    if(isDatatype(tk.type)){
    	node->child1 = iden();
    	node->child2 = declartn();
    }
    return node;
}

Node* op3(){
    Node *node = createNode(op3Node); 
    if(tk.type==tokLPAR){ 
        tk = toks[offset++]; 
        node->child1 = expr(); 
    	if(tk.type==tokRPAR){
            tk = toks[offset++];
            return node;
    	}else{
            printf("ERROR: Line %d expects ')'.\n",tk.pos);
            exit(1);
    	}
    }else if(tk.type==tokIDEN){
        tk = toks[offset++];
        return node;
    }else if(tk.type==tokVAL){ 
        tk = toks[offset++]; 
        return node;
    }else{
        printf("ERROR: Invalid expression. Line %d expects '(', an identifier or a value.\n",tk.pos);
        exit(1);
    }
}

Node* op2(){
    Node *node = createNode(op2Node); 
    if(tk.type==tokSUB){
        tk = toks[offset++];
        node->child1 = op2();
        return node;
    }else{
     	node->child1 = op3();
	return node;
    }
}

Node* op1(){
    Node *node = createNode(op1Node); 
    node->child1 = op2(); 
    if(tk.type==tokADD){ 
        tk = toks[offset++];
        node->child2 = op1();
        return node;
    }else if(tk.type==tokSUB){
        tk = toks[offset++];
        node->child2 = op1();
        return node;
    }else{ 
        return node; //empty after op2
    }
}

Node* expr(){
    Node *node = createNode(exprNode); 
    node->child1 = op1();
    if(tk.type==tokMULT){
        tk = toks[offset++];
        node->child2 = expr();
        return node;
    }else if(tk.type==tokDIV){
        tk = toks[offset++];
        node->child2 = expr();
        return node;
    }else{
        return node; //empty after op1
    }
}

Node* relOp(){
    Node *node = createNode(relOpNode);
    if(tk.type==tokSMLR){
	    tk = toks[offset++];
    }else if(tk.type==tokBGGR){
	    tk = toks[offset++];
    }else if(tk.type==tokASMA){
	    tk = toks[offset++];
    }else if(tk.type==tokABIA){
	    tk = toks[offset++];
    }else if(tk.type==tokTSA){
	    tk = toks[offset++];
    }else if(tk.type==tokNTSA){
	    tk = toks[offset++];
    }else{
	    printf("ERROR: Relational operator is missing in line %d.\n",tk.pos);
	exit(1);
    }
}

Node* loop(){
    Node *node = createNode(hoolaNode);
    node->child1 = expr();
    node->child2 = relOp();
    node->child3 = expr();
    
    if(tk.type==tokEXCLA){
	    tk = toks[offset++]; 
    }else{
	    printf("ERROR: Incorrect hoola format in line %d. '!' is required after the expression.\n",tk.pos);
	    exit(1);
    }
    node->child4=stmtset();
    if(tk.type==tokHOOP){
	    tk = toks[offset++]; 
    }else{
	    printf("ERROR: Hoola is not closed properly in line %d.\n", tk.pos);
	    exit(1);
    }
    return node;
}

Node* board(){
    Node *node = createNode(boardNode);

    if(tk.type==tokLBRAC){
        tk = toks[offset++];
        
        if(tk.type==tokHOLE){
            tk = toks[offset++];
            node->child1 = expr();
            node->child2 = relOp();
            node->child3 = expr();
            
            if(tk.type==tokCOLON){
                tk = toks[offset++];
                node->child4 = stmtset();
                node->child5 = condition();

                if(tk.type==tokRBRAC){
                    tk = toks[offset++];
                }else{
                    printf("ERROR: Invalid MATCHBOARD statement in line %d; expecting ']' at the end of condition.\n",tk.pos);
                    exit(1);
                }
                return node;

            }else{
                printf("ERROR: Invalid HOLE statement in line %d. \n",tk.pos);
                exit(1);
            } 
        }else{
            printf("ERROR: Expecting HOLE keyword in line %d. \n",tk.pos);
            exit(1);
        }
    }else{
        printf("ERROR: Invalid MATCHBOARD statement in line %d; expecting '[' after the keyword MATCHBOARD.\n",tk.pos);
        exit(1);
    }

}

Node* condition(){
    Node *node = createNode(conditionNode);

    if(tk.type==tokHOLE){
        tk = toks[offset++];
        if(tk.type==tokCOLON){
            tk = toks[offset++];
            node->child1 = stmtset();
            return node;
        }else{
            // printf("STRING: %s \n", tk.str);
            node->child1 = expr();
            node->child2 = relOp();
            node->child3 = expr();
            if(tk.type==tokCOLON){
                tk = toks[offset++];
                node->child4 = stmtset();
                node->child5 = condition();
                return node;
            }else{
                printf("ERROR: Invalid HOLE statement in line %d. \n",tk.pos);
                exit(1);
            }  
        }
    }else{
        //null
        return node;
    }
    
}

Node* findplaymate(){
    Node *node = createNode(playNode);
}

Node* showout(){
    Node *node = createNode(showNode);
    string tempString;

    // "string" <outstring>
    if(tk.type==tokQUOTE){
        // "string" <outstring>
        tk = toks[offset++];
        //to capture the whole string even if the string has spaces
        tempString = tk.str;
        while(tk.type != tokQUOTE){
	    if(offset==tokcount-1){
	    	printf("ERROR: Invalid SHOW statement in line %d.\n",tk.pos);
	    	exit(1);
        }
            tk = toks[offset++];
            tempString = tempString + " " + tk.str;
        }

        //printf("STRING: %s", tempString.c_str());
        tk = toks[offset++];
        node->child1 = outstring();

        if(tk.type==tokHASH){
            tk = toks[offset++];
        }else{
            //error end with hash
            printf("ERROR: No line terminator(#) or invalid SHOW statement in line %d.\n", tk.pos);
            exit(1);
        }
        return node;
    }else{
        node->child1 = expr();
        node->child2 = outstring();
        
        if(tk.type==tokHASH){
            tk = toks[offset++];
        }else{
            //error end with hash
            printf("ERROR: No line terminator(#) or invalid SHOW statement in line %d.\n", tk.pos);
            exit(1);
        }

        return node;
    }
}

Node* outstring(){
    Node *node = createNode(outstrNode);
    string tempString;
    
    // "string" <outstring>
    if(tk.type==tokCOMMA){
	tk = toks[offset++];
    	if(tk.type==tokQUOTE){
            // "string" <outstring>
            tk = toks[offset++];
            //to capture the whole string even if the string has spaces
            tempString = tk.str;
            while(tk.type != tokQUOTE){
	    	if(offset==tokcount-1){
	    	    printf("ERROR: Invalid SHOW statement in line %d.\n",tk.pos);
	    	    exit(1);
                }
            	tk = toks[offset++];
            	tempString = tempString + " " + tk.str;
            }
        
            //printf("STRING: %s", tempString.c_str());
            tk = toks[offset++];
            node->child1 = outstring();
            return node;
    	}else{
            node->child1 = expr();
            node->child2 = outstring();
            return node;
	}
    }else{
        //tk = toks[offset++];
        return node;
    }
}

Node* collectout(){
    Node *node = createNode(collectNode);
    if (tk.type == tokIDEN) {
        tk = toks[offset++];

        if (tk.type == tokHASH) {
            tk = toks[offset++];
            return node;
        } else {
            printf("ERROR: No line terminator(#) in line %d.\n", tk.pos);
            exit(1);
        }
    }else{
        printf("ERROR: Expecting an identifier, but received %s on line %d \n", tk.str, tk.pos);
        exit(1);
    }

}

Node* assignVal(){
    Node *node = createNode(asgnNode);
            
    if (tk.type == tokAS) {
        tk = toks[offset++];
        
        node->child1 = expr();
        
        if (tk.type == tokHASH) {
            tk = toks[offset++];

            return node;
        } else {
            printf("ERROR: No line terminator(#) in line %d.\n", tk.pos);
            exit(1);
        }
    } else {
        printf("ERROR: expect 'AS', but received %s in line %d \n", tk.str, tk.pos);
        exit(1);
    }

}

Node* statement(){
    Node *node = createNode(stmtmorNode);;
    if(tk.type==tokHOOLA){
	    tk = toks[offset++]; 
	    node->child1 = loop();
	   return node;
    }else if(tk.type==tokBOARD){
	    tk = toks[offset++]; 
	    node->child1 = board();
	   return node;
    }else if(tk.type==tokFIND){
	    tk = toks[offset++]; 
	    node->child1 = findplaymate();
	return node;
    }else if(tk.type==tokSHOW){
	    tk = toks[offset++]; 
	    node->child1 = showout();
	return node;
    }else if(tk.type==tokCOLLECT){
	    tk = toks[offset++]; 
	    node->child1 = collectout();
	return node;
    }else if(tk.type==tokIDEN){
	    tk = toks[offset++]; 
	    node->child1 = assignVal();
	return node;
    }else if(tk.type==tokOPPLAY){	
	    tk = toks[offset++]; 
	    //function
	return node;
    }else{
	    printf("Error: Cannot recognize line %d as a valid statement.\n",tk.pos);
	    exit(1);
    }
}

Node* stmtset(){
    Node *node = createNode(stmtNode);
    if(isStmt(tk.type)){
    	node->child1 = statement();
    	node->child2 = stmtset();
    }
    return node;
}

Node* box(){
    Node *node = createNode(boxNode);
    if(tk.type==tokOPBOX){ //open Program
	   tk = toks[offset++];
    }else{

       	printf("ERROR: Invalid start of the code. %s\n",tk.str);
	exit(1);

    }

    node->child1 = declartn();
    node->child2 = stmtset();

    if(offset<tokcount-1){ //non-stmtset line before CLBOX
	    printf("ERROR: Invalid statement in line %d.\n",tk.pos);
	    exit(1);
    }

    if(tk.type==tokCLBOX){ //close Program
	    tk = toks[offset++];
	return node;
    }else{
   	    printf("ERROR: Invalid closing of the code.\n");
	    exit(1);
    }
    
}

void parser(){
    tk = toks[offset++];
    Node *root = NULL;
    root = box();
    
    if(tk.type=tokEND) //tk is global so tk will be the end node if parsing works fine
	    printf("Successful parsing!\n");
    else{
	    exit(1);
    }
}

int main(int argc, char *argsv[]){
string linestream,linestream2;
    
    ifstream fp1("sample2.txt");
    if(!fp1.is_open()){
	    printf("ERROR: Cannot open file.");
	return 1;
    }

/*----------------------------------------*/
    //READ all the codes
make_tokenmap();
    for(int i=0;getline(fp1,linestream);i++){
	if(scan_code(linestream,lineNum)){
	    printf("Cannot compile the program. \n");	    
	}
	  
	 //printf("%s\n",linestream.c_str());  
	 code += linestream + "\n";
		
    }
    
    toks = (Token*) malloc(tokcount*sizeof(Token));
    lineNum=1;
    
    //GET tokens
    do{
	    tokcount++;
	    toks = (Token*)realloc(toks,tokcount*sizeof(Token)); 
	    toks[tokcount-1]= anlzr(); 
    }while(toks[tokcount-1].type != tokEND);
    
    offset=0;
    lineNum=0;
    parser();
}

