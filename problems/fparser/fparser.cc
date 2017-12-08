//===============================
// Function parser v3.02 by Warp
//===============================

#include "fpconfig.hh"
#include "fparser.hh"
#include "fptypes.hh"
using namespace FUNCTIONPARSERTYPES;

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cassert>
using namespace std;

#ifdef FP_USE_THREAD_SAFE_EVAL_WITH_ALLOCA
#ifndef FP_USE_THREAD_SAFE_EVAL
#define FP_USE_THREAD_SAFE_EVAL
#endif
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


//=========================================================================
// Name handling functions
//=========================================================================
namespace
{
    const unsigned FUNC_AMOUNT = sizeof(Functions)/sizeof(Functions[0]);

    // -1 = (lhs < rhs); 0 = (lhs == rhs); 1 = (lhs > rhs)
    inline int compare(const FuncDefinition& lhs, const NamePtr& rhs)
    {
        for(unsigned i = 0; i < lhs.nameLength; ++i)
        {
            if(i == rhs.nameLength) return 1;
            const char c1 = lhs.name[i], c2 = rhs.name[i];
            if(c1 < c2) return -1;
            if(c2 < c1) return 1;
        }
        return lhs.nameLength < rhs.nameLength ? -1 : 0;
    }

    inline const FuncDefinition* findFunction(const NamePtr& functionName)
    {
        const FuncDefinition* first = Functions;
        const FuncDefinition* last = Functions + FUNC_AMOUNT;

        while(first < last)
        {
            const FuncDefinition* middle = first+(last-first)/2;
            const int comp = compare(*middle, functionName);
            if(comp == 0) return middle;
            if(comp < 0) first = middle+1;
            else last = middle;
        }
        return 0;
    }

    bool addNewNameData(std::set<NameData>& nameData,
                        std::map<NamePtr, const NameData*>& namePtrs,
                        const NameData& newData)
    {
        if(findFunction(NamePtr(&(newData.name[0]),
                                unsigned(newData.name.size()))))
            return false;

        std::set<NameData>::iterator dataIter = nameData.find(newData);

        if(dataIter != nameData.end())
        {
            if(dataIter->type != newData.type) return false;
            namePtrs.erase(NamePtr(&(dataIter->name[0]),
                                   unsigned(dataIter->name.size())));
            nameData.erase(dataIter);
        }

        dataIter = nameData.insert(newData).first;
        namePtrs[NamePtr(&(dataIter->name[0]),
                         unsigned(dataIter->name.size()))] = &(*dataIter);
        return true;
    }

    const char* readIdentifier(const char* ptr)
    {
        static const char A=10, B=11;
        /*  ^ define numeric constants for two-digit numbers
         *    so as not to disturb the layout of this neat table
         */
        static const char tab[0x100] =
        {
            0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, //00-0F
            0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, //10-1F
            0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, //20-2F
            9,9,9,9, 9,9,9,9, 9,9,0,0, 0,0,0,0, //30-3F
            0,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2, //40-4F
            2,2,2,2, 2,2,2,2, 2,2,2,0, 0,0,0,2, //50-5F
            0,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2, //60-6F
            2,2,2,2, 2,2,2,2, 2,2,2,0, 0,0,0,0, //70-7F
            8,8,8,8, 8,8,8,8, 8,8,8,8, 8,8,8,8, //80-8F
            A,A,A,A, A,A,A,A, A,A,A,A, A,A,A,A, //90-9F
            B,B,B,B, B,B,B,B, B,B,B,B, B,B,B,B, //A0-AF
            B,B,B,B, B,B,B,B, B,B,B,B, B,B,B,B, //B0-BF
            0,0,4,4, 4,4,4,4, 4,4,4,4, 4,4,4,4, //C0-CF
            4,4,4,4, 4,4,4,4, 4,4,4,4, 4,4,4,4, //D0-DF
            5,3,3,3, 3,3,3,3, 3,3,3,3, 3,0,3,3, //E0-EC, EE-EF
            6,1,1,1, 7,0,0,0, 0,0,0,0, 0,0,0,0  //F0-FF
        };
        /* Classes:
         *   9 = digits    (30-39)
         *   2 = A-Z_a-z   (41-5A, 5F, 61-7A)
         *   8 = 80-8F
         *   A = 90-9F
         *   B = A0-BF
         *   4 = C2-DF
         *   5 = E0
         *   3 = E1-EC, EE-EF
         *   6 = F0
         *   1 = F1-F3
         *   7 = F4
         *
         * Allowed multibyte utf8 sequences consist of these class options:
         *   [4]             [8AB]
         *   [5]         [B] [8AB]
         *   [3]       [8AB] [8AB]
         *   [6] [AB]  [8AB] [8AB]
         *   [1] [8AB] [8AB] [8AB]
         *   [7] [8]   [8AB] [8AB]
         * In addition, the first characters may be
         *   [2]
         * And the following characters may be
         *   [92]
         * These may never begin the character:
         *   [08AB]
         *
         * The numberings are such chosen to optimize the
         * following switch-statements for code generation.
         */

        const unsigned char* uptr = (const unsigned char*) ptr;
        switch(tab[uptr[0]])
        {
            case 2: goto loop_2; // A-Z_a-z
            case 5: goto loop_5; // E0
            case 3: goto loop_3; // E1-EC, EE-EF
            case 4: goto loop_4; // C2-DF

            case 1: goto loop_1; // F0-F4 XXX F1-F3
            case 6: goto loop_6; //       XXX F0
            case 7: goto loop_7; //       XXX F4
        }
        return (const char*) uptr;

    loop:
        switch(tab[uptr[0]])
        {
            case 9: // 0-9
            case 2: // A-Z_a-z
            loop_2:
                uptr += 1;
                goto loop;
            case 6: // F0:
            loop_6:
                if(uptr[1] < 0x90 || uptr[1] > 0xBF) break;
                goto len4pos2;
            case 1: // F1-F3:
            loop_1:
                if(uptr[1] < 0x80 || uptr[1] > 0xBF) break;
            len4pos2:
                if(uptr[2] < 0x80 || uptr[2] > 0xBF) break;
                if(uptr[3] < 0x80 || uptr[3] > 0xBF) break;
                uptr += 4;
                goto loop;
            case 7: // F4:
            loop_7:
                if(tab[uptr[1]] != 8) break;
                goto len4pos2;
            case 5: // E0
            loop_5:
                if(tab[uptr[1]] != B) break;
                goto len3pos2;
            case 3: // E1-EC, EE-EF
            loop_3:
                if(uptr[1] < 0x80 || uptr[1] > 0xBF) break;
            len3pos2:
                if(uptr[2] < 0x80 || uptr[2] > 0xBF) break;
                uptr += 3;
                goto loop;
            case 4: // C2-DF
            loop_4:
                if(uptr[1] < 0x80 || uptr[1] > 0xBF) break;
                uptr += 2;
                goto loop;
        }
        return (const char*) uptr;
    }

    bool containsOnlyValidNameChars(const std::string& name)
    {
        if(name.empty()) return false;
        const char* endPtr = readIdentifier(name.c_str());
        return *endPtr == '\0';
    }
}


//=========================================================================
// Data struct implementation
//=========================================================================
FunctionParser::Data::Data(const Data& rhs):
    nameData(rhs.nameData),
    ByteCode(rhs.ByteCode),
    Immed(rhs.Immed),
    StackSize(rhs.StackSize)
{
    Stack.resize(rhs.Stack.size());

    for(std::set<NameData>::const_iterator iter = nameData.begin();
        iter != nameData.end(); ++iter)
    {
        namePtrs[NamePtr(&(iter->name[0]), unsigned(iter->name.size()))] =
            &(*iter);
    }
}


//=========================================================================
// FunctionParser constructors, destructor and assignment
//=========================================================================
FunctionParser::FunctionParser():
    delimiterChar(0),
    parseErrorType(FP_NO_ERROR), evalErrorType(0),
    data(new Data),
    useDegreeConversion(false), isOptimized(false),
    evalRecursionLevel(0)
{
}

FunctionParser::~FunctionParser()
{
    if(--(data->referenceCounter) == 0)
        delete data;
}

FunctionParser::FunctionParser(const FunctionParser& cpy):
    delimiterChar(cpy.delimiterChar),
    parseErrorType(cpy.parseErrorType),
    evalErrorType(cpy.evalErrorType),
    data(cpy.data),
    useDegreeConversion(cpy.useDegreeConversion),
    isOptimized(cpy.isOptimized),
    evalRecursionLevel(0)
{
    ++(data->referenceCounter);
}

FunctionParser& FunctionParser::operator=(const FunctionParser& cpy)
{
    if(data != cpy.data)
    {
        if(--(data->referenceCounter) == 0) delete data;

        delimiterChar = cpy.delimiterChar;
        parseErrorType = cpy.parseErrorType;
        evalErrorType = cpy.evalErrorType;
        data = cpy.data;
        useDegreeConversion = cpy.useDegreeConversion;
        isOptimized = cpy.isOptimized;
        evalRecursionLevel = cpy.evalRecursionLevel;

        ++(data->referenceCounter);
    }

    return *this;
}

void FunctionParser::setDelimiterChar(char c)
{
    delimiterChar = c;
}


//---------------------------------------------------------------------------
// Copy-on-write method
//---------------------------------------------------------------------------
void FunctionParser::CopyOnWrite()
{
    if(data->referenceCounter > 1)
    {
        Data* oldData = data;
        data = new Data(*oldData);
        --(oldData->referenceCounter);
        data->referenceCounter = 1;
    }
}

void FunctionParser::ForceDeepCopy()
{
    CopyOnWrite();
}


//=========================================================================
// User-defined constant and function addition
//=========================================================================
bool FunctionParser::AddConstant(const std::string& name, double value)
{
    if(!containsOnlyValidNameChars(name)) return false;

    CopyOnWrite();
    NameData newData(NameData::CONSTANT, name);
    newData.value = value;
    return addNewNameData(data->nameData, data->namePtrs, newData);
}

bool FunctionParser::AddUnit(const std::string& name, double value)
{
    if(!containsOnlyValidNameChars(name)) return false;

    CopyOnWrite();
    NameData newData(NameData::UNIT, name);
    newData.value = value;
    return addNewNameData(data->nameData, data->namePtrs, newData);
}

bool FunctionParser::AddFunction(const std::string& name,
                                 FunctionPtr ptr, unsigned paramsAmount)
{
    if(!containsOnlyValidNameChars(name)) return false;

    CopyOnWrite();
    NameData newData(NameData::FUNC_PTR, name);
    newData.index = unsigned(data->FuncPtrs.size());

    data->FuncPtrs.push_back(Data::FuncPtrData());
    data->FuncPtrs.back().funcPtr = ptr;
    data->FuncPtrs.back().params = paramsAmount;

    const bool retval = addNewNameData(data->nameData, data->namePtrs, newData);
    if(!retval) data->FuncPtrs.pop_back();
    return retval;
}

bool FunctionParser::CheckRecursiveLinking(const FunctionParser* fp) const
{
    if(fp == this) return true;
    for(unsigned i = 0; i < fp->data->FuncParsers.size(); ++i)
        if(CheckRecursiveLinking(fp->data->FuncParsers[i].parserPtr))
            return true;
    return false;
}

bool FunctionParser::AddFunction(const std::string& name, FunctionParser& fp)
{
    if(!containsOnlyValidNameChars(name) || CheckRecursiveLinking(&fp))
        return false;

    CopyOnWrite();
    NameData newData(NameData::PARSER_PTR, name);
    newData.index = unsigned(data->FuncParsers.size());

    data->FuncParsers.push_back(Data::FuncPtrData());
    data->FuncParsers.back().parserPtr = &fp;
    data->FuncParsers.back().params = unsigned(fp.data->variableRefs.size());

    const bool retval = addNewNameData(data->nameData, data->namePtrs, newData);
    if(!retval) data->FuncParsers.pop_back();
    return retval;
}


//=========================================================================
// Function parsing
//=========================================================================
namespace
{
    // Error messages returned by ErrorMsg():
    const char* const ParseErrorMessage[]=
    {
        "Syntax error",                             // 0
        "Mismatched parenthesis",                   // 1
        "Missing ')'",                              // 2
        "Empty parentheses",                        // 3
        "Syntax error: Operator expected",          // 4
        "Not enough memory",                        // 5
        "An unexpected error occurred. Please make a full bug report "
        "to the author",                            // 6
        "Syntax error in parameter 'Vars' given to "
        "FunctionParser::Parse()",                  // 7
        "Illegal number of parameters to function", // 8
        "Syntax error: Premature end of string",    // 9
        "Syntax error: Expecting ( after function", // 10
        ""
    };
}

// Return parse error message
// --------------------------
const char* FunctionParser::ErrorMsg() const
{
    if(parseErrorType != FP_NO_ERROR) return ParseErrorMessage[parseErrorType];
    return 0;
}


// Parse variables
// ---------------
bool FunctionParser::ParseVariables(const std::string& inputVarString)
{
    if(data->variablesString == inputVarString) return true;

    data->variableRefs.clear();
    data->variablesString = inputVarString;

    const std::string& vars = data->variablesString;
    const unsigned len = unsigned(vars.size());

    unsigned varNumber = VarBegin;

    const char* beginPtr = vars.c_str();
    const char* finalPtr = beginPtr + len;

    while(beginPtr < finalPtr)
    {
        const char* endPtr = readIdentifier(beginPtr);
        if(endPtr == beginPtr) return false;
        if(endPtr != finalPtr && *endPtr != ',') return false;

        NamePtr namePtr(beginPtr, endPtr - beginPtr);

        if(findFunction(namePtr)) return false;

        std::map<NamePtr, const NameData*>::iterator nameIter =
            data->namePtrs.find(namePtr);
        if(nameIter != data->namePtrs.end()) return false;

        if(!(data->variableRefs.insert(make_pair(namePtr, varNumber++)).second))
            return false;

        beginPtr = endPtr + 1;
    }
    return true;
}

// Parse interface functions
// -------------------------
int FunctionParser::Parse(const char* Function, const std::string& Vars,
                          bool useDegrees)
{
    CopyOnWrite();

    if(!ParseVariables(Vars))
    {
        parseErrorType = INVALID_VARS;
        return strlen(Function);
    }

    return ParseFunction(Function, useDegrees);
}

int FunctionParser::Parse(const std::string& Function, const std::string& Vars,
                          bool useDegrees)
{
    CopyOnWrite();

    if(!ParseVariables(Vars))
    {
        parseErrorType = INVALID_VARS;
        return Function.size();
    }

    return ParseFunction(Function.c_str(), useDegrees);
}


// Main parsing function
// ---------------------
int FunctionParser::ParseFunction(const char* function, bool useDegrees)
{
    useDegreeConversion = useDegrees;
    parseErrorType = FP_NO_ERROR;
    isOptimized = false;

    data->ByteCode.clear(); data->ByteCode.reserve(128);
    data->Immed.clear(); data->Immed.reserve(128);
    data->StackSize = StackPtr = 0;

    const char* ptr = CompileExpression(function);
    if(parseErrorType != FP_NO_ERROR) return errorLocation - function;

    assert(ptr); // Should never be null at this point. It's a bug otherwise.
    if(*ptr)
    {
        if(delimiterChar == 0 || *ptr != delimiterChar)
            parseErrorType = EXPECT_OPERATOR;
        return ptr - function;
    }

#ifndef FP_USE_THREAD_SAFE_EVAL
    data->Stack.resize(data->StackSize);
#endif

    return -1;
}


//=========================================================================
// Parsing and bytecode compiling functions
//=========================================================================
inline const char* FunctionParser::SetErrorType(ParseErrorType t,
                                                const char* pos)
{
    parseErrorType = t;
    errorLocation = pos;
    return 0;
}

inline void FunctionParser::incStackPtr()
{
    if(++StackPtr > data->StackSize) ++(data->StackSize);
}

inline void FunctionParser::AddFunctionOpcode(unsigned opcode)
{
    if(useDegreeConversion)
        switch(opcode)
        {
          case cCos:
          case cCosh:
          case cCot:
          case cCsc:
          case cSec:
          case cSin:
          case cSinh:
          case cTan:
          case cTanh:
              data->ByteCode.push_back(cRad);
        }

    data->ByteCode.push_back(opcode);

    if(useDegreeConversion)
        switch(opcode)
        {
          case cAcos:
#ifndef FP_NO_ASINH
          case cAcosh:
          case cAsinh:
          case cAtanh:
#endif
          case cAsin:
          case cAtan:
          case cAtan2:
              data->ByteCode.push_back(cDeg);
        }
}

namespace
{
    inline FunctionParser::ParseErrorType noCommaError(char c)
    {
        return c == ')' ?
            FunctionParser::ILL_PARAMS_AMOUNT : FunctionParser::SYNTAX_ERROR;
    }

    inline FunctionParser::ParseErrorType noParenthError(char c)
    {
        return c == ',' ?
            FunctionParser::ILL_PARAMS_AMOUNT : FunctionParser::MISSING_PARENTH;
    }
}

const char* FunctionParser::CompileIf(const char* function)
{
    if(*function != '(') return SetErrorType(EXPECT_PARENTH_FUNC, function);

    function = CompileExpression(function+1);
    if(!function) return 0;
    if(*function != ',') return SetErrorType(noCommaError(*function), function);

    data->ByteCode.push_back(cIf);
    const unsigned curByteCodeSize = unsigned(data->ByteCode.size());
    data->ByteCode.push_back(0); // Jump index; to be set later
    data->ByteCode.push_back(0); // Immed jump index; to be set later

    --StackPtr;

    function = CompileExpression(function + 1);
    if(!function) return 0;
    if(*function != ',') return SetErrorType(noCommaError(*function), function);

    data->ByteCode.push_back(cJump);
    const unsigned curByteCodeSize2 = unsigned(data->ByteCode.size());
    const unsigned curImmedSize2 = unsigned(data->Immed.size());
    data->ByteCode.push_back(0); // Jump index; to be set later
    data->ByteCode.push_back(0); // Immed jump index; to be set later

    --StackPtr;

    function = CompileExpression(function + 1);
    if(!function) return 0;
    if(*function != ')')
        return SetErrorType(noParenthError(*function), function);

    // Set jump indices
    data->ByteCode[curByteCodeSize] = curByteCodeSize2+1;
    data->ByteCode[curByteCodeSize+1] = curImmedSize2;
    data->ByteCode[curByteCodeSize2] = unsigned(data->ByteCode.size())-1;
    data->ByteCode[curByteCodeSize2+1] = unsigned(data->Immed.size());

    ++function;
    while(isspace(*function)) ++function;
    return function;
}

const char* FunctionParser::CompileFunctionParams(const char* function,
                                                  unsigned requiredParams)
{
    if(*function != '(') return SetErrorType(EXPECT_PARENTH_FUNC, function);

    if(requiredParams > 0)
    {
        function = CompileExpression(function+1);
        if(!function) return 0;

        for(unsigned i = 1; i < requiredParams; ++i)
        {
            if(*function != ',')
                return SetErrorType(noCommaError(*function), function);

            function = CompileExpression(function+1);
            if(!function) return 0;
        }
    }
    else
    {
        ++function;
        while(isspace(*function)) ++function;
    }

    if(*function != ')')
        return SetErrorType(noParenthError(*function), function);
    ++function;
    while(isspace(*function)) ++function;
    return function;
}

const char* FunctionParser::CompileElement(const char* function)
{
    const char c = *function;

    if(c == '(') // Expression in parentheses
    {
        ++function;
        while(isspace(*function)) ++function;
        if(*function == ')') return SetErrorType(EMPTY_PARENTH, function);

        function = CompileExpression(function);
        if(!function) return 0;

        if(*function != ')') return SetErrorType(MISSING_PARENTH, function);

        ++function;
        while(isspace(*function)) ++function;
        return function;
    }

    if(isdigit(c) || c=='.') // Number
    {
        char* endPtr;
        const double val = strtod(function, &endPtr);
        if(endPtr == function) return SetErrorType(SYNTAX_ERROR, function);

        data->Immed.push_back(val);
        data->ByteCode.push_back(cImmed);
        incStackPtr();

        while(isspace(*endPtr)) ++endPtr;
        return endPtr;
    }

    const char* endPtr = readIdentifier(function);
    if(endPtr != function) // Function, variable or constant
    {
        NamePtr name(function, unsigned(endPtr - function));
        while(isspace(*endPtr)) ++endPtr;

        const FuncDefinition* funcDef = findFunction(name);
        if(funcDef) // is function
        {
            if(funcDef->opcode == cIf) // "if" is a special case
                return CompileIf(endPtr);

#ifndef FP_DISABLE_EVAL
            const unsigned requiredParams =
                funcDef->opcode == cEval ?
                unsigned(data->variableRefs.size()) :
                funcDef->params;
#else
            const unsigned requiredParams = funcDef->params;
#endif

            function = CompileFunctionParams(endPtr, requiredParams);
            AddFunctionOpcode(funcDef->opcode);
            return function;
        }

        std::map<NamePtr, unsigned>::iterator varIter =
            data->variableRefs.find(name);
        if(varIter != data->variableRefs.end()) // is variable
        {
            data->ByteCode.push_back(varIter->second);
            incStackPtr();
            return endPtr;
        }

        std::map<NamePtr, const NameData*>::iterator nameIter =
            data->namePtrs.find(name);
        if(nameIter != data->namePtrs.end())
        {
            const NameData* nameData = nameIter->second;
            switch(nameData->type)
            {
              case NameData::CONSTANT:
                  data->Immed.push_back(nameData->value);
                  data->ByteCode.push_back(cImmed);
                  incStackPtr();
                  return endPtr;

              case NameData::UNIT: break;

              case NameData::FUNC_PTR:
                  function = CompileFunctionParams
                      (endPtr, data->FuncPtrs[nameData->index].params);
                  data->ByteCode.push_back(cFCall);
                  data->ByteCode.push_back(nameData->index);
                  return function;

              case NameData::PARSER_PTR:
                  function = CompileFunctionParams
                      (endPtr, data->FuncParsers[nameData->index].params);
                  data->ByteCode.push_back(cPCall);
                  data->ByteCode.push_back(nameData->index);
                  return function;
            }
        }
    }

    if(c == ')') return SetErrorType(MISM_PARENTH, function);
    return SetErrorType(SYNTAX_ERROR, function);
}

const char* FunctionParser::CompilePossibleUnit(const char* function)
{
    const char* endPtr = readIdentifier(function);

    if(endPtr != function)
    {
        NamePtr name(function, unsigned(endPtr - function));
        while(isspace(*endPtr)) ++endPtr;

        std::map<NamePtr, const NameData*>::iterator nameIter =
            data->namePtrs.find(name);
        if(nameIter != data->namePtrs.end())
        {
            const NameData* nameData = nameIter->second;
            if(nameData->type == NameData::UNIT)
            {
                data->Immed.push_back(nameData->value);
                data->ByteCode.push_back(cImmed);
                incStackPtr();
                data->ByteCode.push_back(cMul);
                --StackPtr;
                return endPtr;
            }
        }
    }

    return function;
}

const char* FunctionParser::CompilePow(const char* function)
{
    function = CompileElement(function);
    if(!function) return 0;
    function = CompilePossibleUnit(function);

    if(*function == '^')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileUnaryMinus(function);
        if(!function) return 0;
        data->ByteCode.push_back(cPow);
        --StackPtr;
    }
    return function;
}

const char* FunctionParser::CompileUnaryMinus(const char* function)
{
    const char op = *function;
    if(op == '-' || op == '!')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompilePow(function);
        if(!function) return 0;

        if(op == '-')
        {
            // if we are negating a negation, we can remove both:
            if((data->ByteCode.back() == cNeg))
                data->ByteCode.pop_back();

            // if we are negating a constant, negate the constant itself:
            else if(data->ByteCode.back() == cImmed)
                data->Immed.back() = -data->Immed.back();

            else data->ByteCode.push_back(cNeg);
        }
        else data->ByteCode.push_back(cNot);
    }
    else
        function = CompilePow(function);

    return function;
}

inline const char* FunctionParser::CompileMult(const char* function)
{
    function = CompileUnaryMinus(function);
    if(!function) return 0;

    char op;
    while((op = *function) == '*' || op == '/' || op == '%')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileUnaryMinus(function);
        if(!function) return 0;
        switch(op)
        {
          case '*': data->ByteCode.push_back(cMul); break;
          case '/': data->ByteCode.push_back(cDiv); break;
          case '%': data->ByteCode.push_back(cMod); break;
        }
        --StackPtr;
    }
    return function;
}

inline const char* FunctionParser::CompileAddition(const char* function)
{
    function = CompileMult(function);
    if(!function) return 0;

    char op;
    while((op = *function) == '+' || op == '-')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileMult(function);
        if(!function) return 0;
        data->ByteCode.push_back(op=='+' ? cAdd : cSub);
        --StackPtr;
    }
    return function;
}

namespace
{
    inline int getComparisonOpcode(const char*& f)
    {
        switch(*f)
        {
          case '=':
              ++f; return cEqual;

          case '!':
              if(f[1] == '=') { f += 2; return cNEqual; }
              return -1; // If '=' does not follow '!', a syntax error will
                         // be generated at the outermost parsing level

          case '<':
              if(f[1] == '=') { f += 2; return cLessOrEq; }
              ++f; return cLess;

          case '>':
              if(f[1] == '=') { f += 2; return cGreaterOrEq; }
              ++f; return cGreater;
        }
        return -1;
    }
}

const char* FunctionParser::CompileComparison(const char* function)
{
    function = CompileAddition(function);
    if(!function) return 0;

    int opCode;
    while((opCode = getComparisonOpcode(function)) >= 0)
    {
        while(isspace(*function)) ++function;
        function = CompileAddition(function);
        if(!function) return 0;
        data->ByteCode.push_back(opCode);
        --StackPtr;
    }
    return function;
}

inline const char* FunctionParser::CompileAnd(const char* function)
{
    function = CompileComparison(function);
    if(!function) return 0;

    while(*function == '&')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileComparison(function);
        if(!function) return 0;
        data->ByteCode.push_back(cAnd);
        --StackPtr;
    }
    return function;
}

const char* FunctionParser::CompileExpression(const char* function)
{
    while(isspace(*function)) ++function;
    function = CompileAnd(function);
    if(!function) return 0;

    while(*function == '|')
    {
        ++function;
        while(isspace(*function)) ++function;
        function = CompileAnd(function);
        if(!function) return 0;
        data->ByteCode.push_back(cOr);
        --StackPtr;
    }
    return function;
}


//===========================================================================
// Function evaluation
//===========================================================================
namespace
{
    inline int doubleToInt(double d)
    {
        return d<0 ? -int((-d)+.5) : int(d+.5);
    }

    inline double Min(double d1, double d2)
    {
        return d1<d2 ? d1 : d2;
    }
    inline double Max(double d1, double d2)
    {
        return d1>d2 ? d1 : d2;
    }


    inline double DegreesToRadians(double degrees)
    {
        return degrees*(M_PI/180.0);
    }
    inline double RadiansToDegrees(double radians)
    {
        return radians*(180.0/M_PI);
    }
}

double FunctionParser::Eval(const double* Vars)
{
    const unsigned* const ByteCode = &(data->ByteCode[0]);
    const double* const Immed = &(data->Immed[0]);
    const unsigned ByteCodeSize = unsigned(data->ByteCode.size());
    unsigned IP, DP=0;
    int SP=-1;

#ifdef FP_USE_THREAD_SAFE_EVAL
#ifdef FP_USE_THREAD_SAFE_EVAL_WITH_ALLOCA
    double* const Stack = (double*)alloca(data->StackSize*sizeof(double));
#else
    std::vector<double> Stack(data->StackSize);
#endif
#else
    std::vector<double>& Stack = data->Stack;
#endif

    for(IP=0; IP<ByteCodeSize; ++IP)
    {
        switch(ByteCode[IP])
        {
// Functions:
          case   cAbs: Stack[SP] = fabs(Stack[SP]); break;
          case  cAcos: if(Stack[SP] < -1 || Stack[SP] > 1)
                       { evalErrorType=4; return 0; }
                       Stack[SP] = acos(Stack[SP]); break;
#ifndef FP_NO_ASINH
          case cAcosh: Stack[SP] = acosh(Stack[SP]); break;
#endif
          case  cAsin: if(Stack[SP] < -1 || Stack[SP] > 1)
                       { evalErrorType=4; return 0; }
                       Stack[SP] = asin(Stack[SP]); break;
#ifndef FP_NO_ASINH
          case cAsinh: Stack[SP] = asinh(Stack[SP]); break;
#endif
          case  cAtan: Stack[SP] = atan(Stack[SP]); break;
          case cAtan2: Stack[SP-1] = atan2(Stack[SP-1], Stack[SP]);
                       --SP; break;
#ifndef FP_NO_ASINH
          case cAtanh: Stack[SP] = atanh(Stack[SP]); break;
#endif
          case  cCeil: Stack[SP] = ceil(Stack[SP]); break;
          case   cCos: Stack[SP] = cos(Stack[SP]); break;
          case  cCosh: Stack[SP] = cosh(Stack[SP]); break;

          case   cCot:
              {
                  double t = tan(Stack[SP]);
                  if(t == 0) { evalErrorType=1; return 0; }
                  Stack[SP] = 1/t; break;
              }
          case   cCsc:
              {
                  double s = sin(Stack[SP]);
                  if(s == 0) { evalErrorType=1; return 0; }
                  Stack[SP] = 1/s; break;
              }


#ifndef FP_DISABLE_EVAL
          case  cEval:
              {
                  const unsigned varAmount =
                      unsigned(data->variableRefs.size());
                  double retVal = 0;
                  if(evalRecursionLevel == FP_EVAL_MAX_REC_LEVEL)
                  {
                      evalErrorType = 5;
                  }
                  else
                  {
                      ++evalRecursionLevel;
#ifndef FP_USE_THREAD_SAFE_EVAL
                      std::vector<double> tmpStack(Stack.size());
                      data->Stack.swap(tmpStack);
                      retVal = Eval(&tmpStack[SP - varAmount + 1]);
                      data->Stack.swap(tmpStack);
#else
                      retVal = Eval(&Stack[SP - varAmount + 1]);
#endif
                      --evalRecursionLevel;
                  }
                  SP -= varAmount-1;
                  Stack[SP] = retVal;
                  break;
              }
#endif

          case   cExp: Stack[SP] = exp(Stack[SP]); break;
          case cFloor: Stack[SP] = floor(Stack[SP]); break;

          case    cIf:
              {
                  unsigned jumpAddr = ByteCode[++IP];
                  unsigned immedAddr = ByteCode[++IP];
                  if(doubleToInt(Stack[SP]) == 0)
                  {
                      IP = jumpAddr;
                      DP = immedAddr;
                  }
                  --SP; break;
              }

          case   cInt: Stack[SP] = floor(Stack[SP]+.5); break;
          case   cLog: if(Stack[SP] <= 0) { evalErrorType=3; return 0; }
                       Stack[SP] = log(Stack[SP]); break;
          case cLog10: if(Stack[SP] <= 0) { evalErrorType=3; return 0; }
                       Stack[SP] = log10(Stack[SP]); break;
          case   cMax: Stack[SP-1] = Max(Stack[SP-1], Stack[SP]);
                       --SP; break;
          case   cMin: Stack[SP-1] = Min(Stack[SP-1], Stack[SP]);
                       --SP; break;
          case   cSec:
              {
                  double c = cos(Stack[SP]);
                  if(c == 0) { evalErrorType=1; return 0; }
                  Stack[SP] = 1/c; break;
              }
          case   cSin: Stack[SP] = sin(Stack[SP]); break;
          case  cSinh: Stack[SP] = sinh(Stack[SP]); break;
          case  cSqrt: if(Stack[SP] < 0) { evalErrorType=2; return 0; }
                       Stack[SP] = sqrt(Stack[SP]); break;
          case   cTan: Stack[SP] = tan(Stack[SP]); break;
          case  cTanh: Stack[SP] = tanh(Stack[SP]); break;


// Misc:
          case cImmed: Stack[++SP] = Immed[DP++]; break;
          case  cJump: DP = ByteCode[IP+2];
                       IP = ByteCode[IP+1];
                       break;

// Operators:
          case   cNeg: Stack[SP] = -Stack[SP]; break;
          case   cAdd: Stack[SP-1] += Stack[SP]; --SP; break;
          case   cSub: Stack[SP-1] -= Stack[SP]; --SP; break;
          case   cMul: Stack[SP-1] *= Stack[SP]; --SP; break;
          case   cDiv: if(Stack[SP] == 0) { evalErrorType=1; return 0; }
                       Stack[SP-1] /= Stack[SP]; --SP; break;
          case   cMod: if(Stack[SP] == 0) { evalErrorType=1; return 0; }
                       Stack[SP-1] = fmod(Stack[SP-1], Stack[SP]);
                       --SP; break;
          case   cPow: Stack[SP-1] = pow(Stack[SP-1], Stack[SP]);
                       --SP; break;

#ifdef FP_EPSILON
          case cEqual: Stack[SP-1] =
                           (fabs(Stack[SP-1]-Stack[SP]) <= FP_EPSILON);
                       --SP; break;
          case cNEqual: Stack[SP-1] =
                            (fabs(Stack[SP-1] - Stack[SP]) >= FP_EPSILON);
                       --SP; break;
          case  cLess: Stack[SP-1] = (Stack[SP-1] < Stack[SP]-FP_EPSILON);
                       --SP; break;
          case  cLessOrEq: Stack[SP-1] = (Stack[SP-1] <= Stack[SP]+FP_EPSILON);
                       --SP; break;
          case cGreater: Stack[SP-1] = (Stack[SP-1]-FP_EPSILON > Stack[SP]);
                         --SP; break;
          case cGreaterOrEq: Stack[SP-1] =
                                 (Stack[SP-1]+FP_EPSILON >= Stack[SP]);
                         --SP; break;
#else
          case cEqual: Stack[SP-1] = (Stack[SP-1] == Stack[SP]);
                       --SP; break;
          case cNEqual: Stack[SP-1] = (Stack[SP-1] != Stack[SP]);
                       --SP; break;
          case  cLess: Stack[SP-1] = (Stack[SP-1] < Stack[SP]);
                       --SP; break;
          case  cLessOrEq: Stack[SP-1] = (Stack[SP-1] <= Stack[SP]);
                       --SP; break;
          case cGreater: Stack[SP-1] = (Stack[SP-1] > Stack[SP]);
                         --SP; break;
          case cGreaterOrEq: Stack[SP-1] = (Stack[SP-1] >= Stack[SP]);
                         --SP; break;
#endif

          case   cAnd: Stack[SP-1] =
                           (doubleToInt(Stack[SP-1]) &&
                            doubleToInt(Stack[SP]));
                       --SP; break;
          case    cOr: Stack[SP-1] =
                           (doubleToInt(Stack[SP-1]) ||
                            doubleToInt(Stack[SP]));
                       --SP; break;
          case   cNot: Stack[SP] = !doubleToInt(Stack[SP]); break;

// Degrees-radians conversion:
          case   cDeg: Stack[SP] = RadiansToDegrees(Stack[SP]); break;
          case   cRad: Stack[SP] = DegreesToRadians(Stack[SP]); break;

// User-defined function calls:
          case cFCall:
              {
                  unsigned index = ByteCode[++IP];
                  unsigned params = data->FuncPtrs[index].params;
                  double retVal =
                      data->FuncPtrs[index].funcPtr(&Stack[SP-params+1]);
                  SP -= int(params)-1;
                  Stack[SP] = retVal;
                  break;
              }

          case cPCall:
              {
                  unsigned index = ByteCode[++IP];
                  unsigned params = data->FuncParsers[index].params;
                  double retVal =
                      data->FuncParsers[index].parserPtr->Eval
                      (&Stack[SP-params+1]);
                  SP -= int(params)-1;
                  Stack[SP] = retVal;
                  const int error =
                      data->FuncParsers[index].parserPtr->EvalError();
                  if(error)
                  {
                      evalErrorType = error;
                      return 0;
                  }
                  break;
              }


#ifdef FP_SUPPORT_OPTIMIZER
          case   cVar: break; // Paranoia. These should never exist
          case   cDup: Stack[SP+1] = Stack[SP]; ++SP; break;
          case   cInv:
              if(Stack[SP] == 0.0) { evalErrorType=1; return 0; }
              Stack[SP] = 1.0/Stack[SP];
              break;
#endif

// Variables:
          default:
              Stack[++SP] = Vars[ByteCode[IP]-VarBegin];
        }
    }

    evalErrorType=0;
    return Stack[SP];
}


#ifdef FUNCTIONPARSER_SUPPORT_DEBUG_OUTPUT
#include <iomanip>
namespace
{
    inline void printHex(std::ostream& dest, unsigned n)
    {
        dest.width(8); dest.fill('0'); std::hex(dest); //uppercase(dest);
        dest << n;
    }
}

void FunctionParser::PrintByteCode(std::ostream& dest) const
{
    dest << "Size of stack: " << data->StackSize << "\n";

    const std::vector<unsigned>& ByteCode = data->ByteCode;
    const std::vector<double>& Immed = data->Immed;

    for(unsigned IP = 0, DP = 0; IP < ByteCode.size(); ++IP)
    {
        printHex(dest, IP);
        dest << ": ";

        unsigned opcode = ByteCode[IP];

        switch(opcode)
        {
          case cIf:
              dest << "jz\t";
              printHex(dest, ByteCode[IP+1]+1);
              dest << endl;
              IP += 2;
              break;

          case cJump:
              dest << "jump\t";
              printHex(dest, ByteCode[IP+1]+1);
              dest << endl;
              IP += 2;
              break;
          case cImmed:
              dest.precision(10);
              dest << "push\t" << Immed[DP++] << endl;
              break;

          case cFCall:
              {
                  const unsigned index = ByteCode[++IP];
                  std::set<NameData>::const_iterator iter =
                      data->nameData.begin();
                  while(iter->type != NameData::FUNC_PTR ||
                        iter->index != index)
                      ++iter;
                  dest << "fcall\t" << iter->name
                       << " (" << data->FuncPtrs[index].params << ")" << endl;
                  break;
              }

          case cPCall:
              {
                  const unsigned index = ByteCode[++IP];
                  std::set<NameData>::const_iterator iter =
                      data->nameData.begin();
                  while(iter->type != NameData::PARSER_PTR ||
                        iter->index != index)
                      ++iter;
                  dest << "pcall\t" << iter->name
                       << " (" << data->FuncParsers[index].params
                       << ")" << endl;
                  break;
              }

          default:
              if(OPCODE(opcode) < VarBegin)
              {
                  string n;
                  unsigned params = 1;
                  switch(opcode)
                  {
                    case cNeg: n = "neg"; break;
                    case cAdd: n = "add"; break;
                    case cSub: n = "sub"; break;
                    case cMul: n = "mul"; break;
                    case cDiv: n = "div"; break;
                    case cMod: n = "mod"; break;
                    case cPow: n = "pow"; break;
                    case cEqual: n = "eq"; break;
                    case cNEqual: n = "neq"; break;
                    case cLess: n = "lt"; break;
                    case cLessOrEq: n = "le"; break;
                    case cGreater: n = "gt"; break;
                    case cGreaterOrEq: n = "ge"; break;
                    case cAnd: n = "and"; break;
                    case cOr: n = "or"; break;
                    case cNot: n = "not"; break;
                    case cDeg: n = "deg"; break;
                    case cRad: n = "rad"; break;

#ifndef FP_DISABLE_EVAL
                    case cEval: n = "call\t0"; break;
#endif

#ifdef FP_SUPPORT_OPTIMIZER
                    case cVar: n = "(var)"; break;
                    case cDup: n = "dup"; break;
                    case cInv: n = "inv"; break;
#endif

                    default:
                        n = Functions[opcode-cAbs].name;
                        params = Functions[opcode-cAbs].params;
                  }
                  dest << n;
                  if(params != 1) dest << " (" << params << ")";
                  dest << endl;
              }
              else
              {
                  dest << "push\tVar" << opcode-VarBegin << endl;
              }
        }
    }
}
#endif


#ifndef FP_SUPPORT_OPTIMIZER
void FunctionParser::MakeTree(void *) const {}
void FunctionParser::Optimize()
{
    // Do nothing if no optimizations are supported.
}
#endif
