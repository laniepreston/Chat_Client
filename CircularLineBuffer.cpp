//
// Created by Floor Kouwenberg on 19-2-2019.
//

#include "CircularLineBuffer.h"


/**
     * This method writes the given number of characters into the buffer,
     * starting at the next free location.
     * If there is not enough space left in the buffer, it writes nothing into the buffer and returns false.
     *
     * @param chars Pointer to the characters to write into the buffer.
     * @param nchars The number of characters to write.
     * @return False if there was not enough space in the buffer. True otherwise.
     */
bool CircularLineBuffer::_writeChars(const char *chars, size_t nchars){
    if(freeSpace() < nchars){
        printf("Buffer is full\n");
        return false;
    }
    int counter = 0;
    int nfi = nextFreeIndex();
    for(int x = nfi; x<nfi+nchars; x++){
        buffer[x % bufferSize] = chars[counter];
        counter++;
    }
    count = count + nchars;
    return true;

}

/**
 * This method reads a line from the buffer,
 * starting from location 'start'.
 * If there is no complete line (no '\n') in the buffer, this method returns an empty string.
 *
 * @return The next string from the buffer. Returns an empty string if the buffer is empty.
 */
std::string CircularLineBuffer::_readLine(){
    if (hasLine()== false){
        return "";
    }

    int nl = findNewline();
    if (nl == -1) {
        printf("Could not find a new line\n");
        return "";
    }

    std::string output = "";
    output=+ &buffer[start % bufferSize];

    start =(start + nl) % bufferSize;
    count =count - nl;
    return output;

}


/**
     * @return The amount of free space in the buffer in number of characters.
     */
int CircularLineBuffer::freeSpace(){
    return bufferSize-count;
}



/**
 *
 * @return true if and only if (iff) the buffer is full.
 */
bool CircularLineBuffer::isFull(){
    return count == bufferSize;
}



/**
 *
 * @return true if and only if (iff) the buffer is empty.
 */
bool CircularLineBuffer::isEmpty(){
    return count == 0;
}

/**
 * This method should return the next free spot in the buffer as seen from the current value of 'start'.
 *
 * For example, consider the following buffer:
 *  S
 * [H,E,L,L,O,\n,-,-]
 * Here 'S' points to the start of the buffer contents, and '-' indicates an empty space in the buffer.
 * For this buffer, nextFreeIndex should return 6,
 * because it is the first free position in the buffer.
 *
 * If the buffer is full, the behavior is undefined.
 *
 * @return The index of the first free position in the buffer.
 */
int CircularLineBuffer::nextFreeIndex(){
    if (isFull()){
        return -1;
    }
    return (start+count) % bufferSize;
}

/**
     * The position of the next newline character (\n), as seen from the current value of 'start'.
     *
     * For example, consider the following buffer:
     *     S
     * [\n,H,I,\n,-,-]
     * Here 'S' points to the start of the buffer contents, and '-' indicates an empty space in the buffer.
     * For this buffer, findNewline should return 3, because,
     * when starting from S, index 3 contains the first '\n' character.
     *
     * @return The position of the next newline character (\n), as seen from the current value of 'start'.
     */
int CircularLineBuffer::findNewline(){
    for(int p = start; p < (start + count); p++) {
        if (buffer[p % bufferSize] == '\n') {
            return p - start + 1;
        }
    }
    return -1;
}

/**
 * Checks if there is a complete line in the buffer.
 * You can make your life easier by implementing this method using the method above.
 *
 * @return true if and only if (iff) there is at least one complete line in the buffer.
 */
bool CircularLineBuffer::hasLine(){
    return findNewline() != -1;
}
