#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_iterator.hpp"
/*
 * Prints identifier of given sc-element.
 * If element has not identifier, prints its address in sc-memory
 * If element is arc prints it in (a -> b) form recursively.
*/
void printEl(const std::unique_ptr<ScMemoryContext>& context, ScAddr element);
/*
 * Prints content of given sc-link
*/
