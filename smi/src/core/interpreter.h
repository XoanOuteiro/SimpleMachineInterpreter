#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include "ast.h"
#include "smierror.h"

namespace smi::interpreter {

typedef enum {
    INTERPRETER_OK = 0,
    INTERPRETER_ERR_UNDEFINED_LABEL,
    INTERPRETER_ERR_INCOMPATIBLE_LABEL_TYPE,
    INTERPRETER_ERR_INVALID_TOKEN,
    INTERPRETER_ERR_UNEXPECTED_TOKEN
} InterpreterError;

class Interpreter {
   private:
    std::vector<std::string> memory_keys;
    std::unordered_map<std::string, uint16_t> memory;
    std::unordered_map<std::string, uint16_t> labels;
    int executedInstructions;

   protected:
    std::unique_ptr<Program> program;
    int pc;
    bool cmp;

    int eval(Node* node);
    void evalLabel(Label* ls);
    int evalInstruction(Instruction* inst);
    void evalAssignment(Assignment* as);

    bool isMemoryLabel(const std::string& label) { return memory.find(label) != memory.end(); }
    bool isCodeLabel(const std::string& label) { return labels.find(label) != labels.end(); }

   public:
    Interpreter() : executedInstructions(0) {}

    int eval(const std::string& code);
    const std::vector<std::string>& getMemoryKeys();
    uint16_t getMemoryValue(std::string key);
    int getExecutedInstructions() const { return this->executedInstructions; };
};

}  // namespace smi::interpreter

#define THROW_LABEL_NOT_FOUND_EXC(label, pos, line, column) \
    std::ostringstream buffer;                              \
    buffer << "Undefined label '" << label;                 \
    buffer << "'";                                          \
    smi::error::setLastError(pos, line, column, label.length(), buffer.str());

#define THROW_INCOMPATIBLE_LABEL_TYPE_EXC(label, pos, line, column) \
    std::ostringstream buffer;                                      \
    buffer << "Incompatible label '" << label;                      \
    buffer << "'";                                                  \
    smi::error::setLastError(pos, line, column, label.length(), buffer.str());

#endif
