#pragma once

#include <string>

namespace pinedb
{

    /**  Language codes to be used with the PineDB class */
    enum class LanguageCode
    {
        EN,
        DE,
        ES,
        FR
    };

    /**
     * @brief A class for saying hello in multiple languages
     */
    class PineDB
    {
        std::string name;

      public:
        /**
         * @brief Creates a new pinedb
         * @param name the name to greet
         */
        PineDB(std::string name);

        /**
         * @brief Creates a localized string containing the greeting
         * @param lang the language to greet in
         * @return a string containing the greeting
         */
        std::string greet(LanguageCode lang = LanguageCode::EN) const;
    };

} // namespace pinedb
