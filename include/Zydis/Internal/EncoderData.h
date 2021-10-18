/***************************************************************************************************

  Zyan Disassembler Library (Zydis)

  Original Author : Mappa

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

***************************************************************************************************/

#ifndef ZYDIS_INTERNAL_ENCODERDATA_H
#define ZYDIS_INTERNAL_ENCODERDATA_H

#include <Zycore/Defines.h>
#include <Zydis/Mnemonic.h>
#include <Zydis/Internal/SharedData.h>

/**
 * Defines the `ZydisWidthFlag` enum.
 */
typedef enum ZydisWidthFlag_
{
    ZYDIS_WIDTH_INVALID = 0x00,
    ZYDIS_WIDTH_16      = 0x01,
    ZYDIS_WIDTH_32      = 0x02,
    ZYDIS_WIDTH_64      = 0x04,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_WIDTH_MAX_VALUE = (ZYDIS_WIDTH_64 | (ZYDIS_WIDTH_64 - 1)),
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_WIDTH_REQUIRED_BITS = ZYAN_BITS_TO_REPRESENT(ZYDIS_WIDTH_MAX_VALUE)
} ZydisWidthFlag;

/**
 * Defines the `ZydisMandatoryPrefix` enum.
 */
typedef enum ZydisMandatoryPrefix_
{
    ZYDIS_MANDATORY_PREFIX_NONE,
    ZYDIS_MANDATORY_PREFIX_66,
    ZYDIS_MANDATORY_PREFIX_F2,
    ZYDIS_MANDATORY_PREFIX_F3,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_MANDATORY_PREFIX_MAX_VALUE = ZYDIS_MANDATORY_PREFIX_F3,
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_MANDATORY_PREFIX_REQUIRED_BITS = ZYAN_BITS_TO_REPRESENT(ZYDIS_MANDATORY_PREFIX_MAX_VALUE)
} ZydisMandatoryPrefix;

/**
 * Defines the `ZydisVectorLength` enum.
 */
typedef enum ZydisVectorLength_
{
    ZYDIS_VECTOR_LENGTH_INVALID,
    ZYDIS_VECTOR_LENGTH_128,
    ZYDIS_VECTOR_LENGTH_256,
    ZYDIS_VECTOR_LENGTH_512,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_VECTOR_LENGTH_MAX_VALUE = ZYDIS_VECTOR_LENGTH_512,
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_VECTOR_LENGTH_REQUIRED_BITS = ZYAN_BITS_TO_REPRESENT(ZYDIS_VECTOR_LENGTH_MAX_VALUE)
} ZydisVectorLength;

/**
 * Defines the `ZydisSizeHint` enum.
 */
typedef enum ZydisSizeHint_
{
    ZYDIS_SIZE_HINT_NONE,
    ZYDIS_SIZE_HINT_ASZ,
    ZYDIS_SIZE_HINT_OSZ,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_SIZE_HINT_MAX_VALUE = ZYDIS_SIZE_HINT_OSZ,
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_SIZE_HINT_REQUIRED_BITS = ZYAN_BITS_TO_REPRESENT(ZYDIS_SIZE_HINT_MAX_VALUE)
} ZydisSizeHint;

/**
 * Defines the `ZydisEncoderLookupEntry` enum.
 */
typedef struct ZydisEncoderLookupEntry_
{
    /**
     * Index to main array of `ZydisEncodableInstruction`.
     */
    ZyanU16 encoder_reference;
    /**
     * The number of entries.
     */
    ZyanU8 instruction_count;
} ZydisEncoderLookupEntry;

#pragma pack(push, 1)

/**
 * Defines the `ZydisEncodableInstruction` enum.
 */
typedef struct ZydisEncodableInstruction_
{
    /**
     * Index to one of decoder's instruction definition arrays.
     */
    ZyanU16 instruction_reference;
    /**
     * The instruction-opcode.
     */
    ZyanU8 opcode;
    /**
     * The mandatory ModR/M value.
     */
    ZyanU8 modrm;
    /**
     * The instruction-encoding.
     */
    ZyanU8 encoding                 ZYAN_BITFIELD(ZYDIS_INSTRUCTION_ENCODING_REQUIRED_BITS);
    /**
     * The opcode map.
     */
    ZyanU8 opcode_map               ZYAN_BITFIELD(ZYDIS_OPCODE_MAP_REQUIRED_BITS);
    /**
     * The combination of allowed processor modes.
     */
    ZyanU8 modes                    ZYAN_BITFIELD(ZYDIS_WIDTH_REQUIRED_BITS);
    /**
     * The combination of allowed address sizes.
     */
    ZyanU8 address_sizes            ZYAN_BITFIELD(ZYDIS_WIDTH_REQUIRED_BITS);
    /**
     * The combination of allowed operand sizes.
     */
    ZyanU8 operand_sizes            ZYAN_BITFIELD(ZYDIS_WIDTH_REQUIRED_BITS);
    /**
     * The mandatory prefix.
     */
    ZyanU8 mandatory_prefix         ZYAN_BITFIELD(ZYDIS_MANDATORY_PREFIX_REQUIRED_BITS);
    /**
     * True if `REX.W` is required for this definition.
     */
    ZyanU8 rex_w                    ZYAN_BITFIELD(1);
    /**
     * The vector length.
     */
    ZyanU8 vector_length            ZYAN_BITFIELD(ZYDIS_MANDATORY_PREFIX_REQUIRED_BITS);
    /**
     * The accepted sizing hint.
     */
    ZyanU8 accepts_hint             ZYAN_BITFIELD(ZYDIS_SIZE_HINT_REQUIRED_BITS);
} ZydisEncodableInstruction;

#pragma pack(pop)

/**
 * Fetches array of `ZydisEncodableInstruction` structures and its size for given instruction 
 * mnemonic. 
 *
 * @param   mnemonic    Instruction mnemonic.
 * @param   instruction This variable will receive a pointer to the array of 
 *                      `ZydisEncodableInstruction` structures.
 *
 * @return  Entry count (0 if function failed).
 */
ZyanU8 ZydisGetEncodableInstructions(ZydisMnemonic mnemonic, 
    const ZydisEncodableInstruction **instruction);

#endif /* ZYDIS_INTERNAL_ENCODERDATA_H */