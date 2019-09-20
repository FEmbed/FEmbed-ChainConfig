/*
 * ChainConfig.h
 *
 *  Created on: 2019年9月20日
 *      Author: Gene Kong
 */

#ifndef FEMBED_CHAINCONFIG_SRC_CHAINCONFIG_H_
#define FEMBED_CHAINCONFIG_SRC_CHAINCONFIG_H_

#include <stdint.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#endif

namespace FEmbed {

/**
 * Configuration option rules.
 */
typedef enum : uint8_t {
    ChainConfigOptionLabel_Required,//!< ChainConfigOptionLabel_Required
    ChainConfigOptionLabel_Optional,//!< ChainConfigOptionLabel_Optional
    ChainConfigOptionLabel_Repeated,//!< ChainConfigOptionLabel_Repeated
    ChainConfigOptionLabel_None     //!< ChainConfigOptionLabel_None
} ChainConfigOptionLabel;

/**
 * Configuration option types.
 */
typedef enum : uint8_t{
    ChainConfigOptionType_Integer,
    ChainConfigOptionType_UInteger,
    ChainConfigOptionType_Decimal,
    ChainConfigOptionType_Enum,
    ChainConfigOptionType_String,
    ChainConfigOptionType_Bytes
} ChainConfigOptionType;

struct ChainConfigOptionFiledDescription {
    const char *name;
    const char *help;

    ChainConfigOptionLabel label;
    ChainConfigOptionType type;
};

struct ChainConfigOptionDescription {
    const char *name;
    const char *description;
    const char *result_help;

    uint32_t timeout;           ///< Command work timeout.

    const ChainConfigOptionFiledDescription *fields;
    uint32_t n_fields_num;

    uint32_t option_identify;
};

struct ChainConfigDescription {
    const char *name;

    /** Command Identify value, high 16-bit for group, low 16-bit for type. */
    uint32_t            indetify;

    const ChainConfigOptionDescription *options;
    uint32_t n_options_num;
};

struct ChainConfigOptionFiledObject {
    union {
        uint64_t _uint;
        int64_t  _sint;
        double  _decimal;
        const char *_enum_str;       ///< Enum/String(free for string)
        struct {                     ///< Bytes(free)
            uint8_t *_bytes;
            int _bytes_len;
        };
    };
    uint8_t _is_valid;               ///< indicate this field is valid or not.
};

class ChainConfig
{
 public:
    ChainConfig();
    virtual ~ChainConfig();

    /**
     * Set Module Configuration.
     *
     * Don't free any field object in ChainConfig implement.
     *
     * @param option_identify option identify.
     * @param fields fields list.
     * @param fields_num fields number.
     * @return execute return.
     */
    virtual int setChainConfig(
                    uint32_t option_identify,
                    ChainConfigOptionFiledObject* fields,
                    uint32_t fields_num) = 0;

    /**
     * Get Module Configuration.
     * @param option_identify
     * @param fields
     * @param fields_num
     * @return execute return.
     */
    virtual int getChainConfig(
                    uint32_t option_identify,
                    ChainConfigOptionFiledObject** fields,
                    uint32_t *fields_num) = 0;
    /**
     * Get Module Configuration Description.
     * @return ChainConfigDescription object.
     */
    virtual const ChainConfigDescription *getConfigDescription() = 0;

    virtual bool saveConfig();
    virtual bool restoreConfig();
    virtual bool resetConfig();
};

} /* namespace FEmbed */

#endif /* FEMBED_CHAINCONFIG_SRC_CHAINCONFIG_H_ */
