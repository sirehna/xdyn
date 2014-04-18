/**********************************************************************************************************************
**         __________              ___                              ________                                         **
**         \______   \_____     __| _/ _____  _____     ____       /  _____/ _____     _____    ____    ______       **
**          |       _/\__  \   / __ | /     \ \__  \   /    \     /   \  ___ \__  \   /     \ _/ __ \  /  ___/       **
**          |    |   \ / __ \_/ /_/ ||  Y Y  \ / __ \_|   |  \    \    \_\  \ / __ \_|  Y Y  \\  ___/  \___ \        **
**          |____|_  /(____  /\____ ||__|_|  /(____  /|___|  /     \______  /(____  /|__|_|  / \___  \/____  \       **
**                 \/      \/      \/      \/      \/      \/             \/      \/       \/      \/      \/        **
**                                                         2012                                                      **
**********************************************************************************************************************/

#include "OptionPrinter.hpp"


namespace rad
{
    OptionPrinter::OptionPrinter() :  options_(std::vector<CustomOptionDescription>()),
            positionalOptions_(std::vector<CustomOptionDescription>())
    {

    }

//---------------------------------------------------------------------------------------------------------------------
  void OptionPrinter::addOption(const CustomOptionDescription& optionDesc)
  {
    optionDesc.isPositional_ ? positionalOptions_.push_back(optionDesc) : options_.push_back(optionDesc);

  }

//---------------------------------------------------------------------------------------------------------------------
  std::string OptionPrinter::usage()
  {
    std::stringstream usageDesc;
    /** simple flags that have a short version
     */
    bool firstShortOption = true;
    usageDesc << "[";
    for (std::vector<rad::CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( it->hasShort_ && ! it->hasArgument_ && ! it->required_ )
      {
        if (firstShortOption)
        {
          usageDesc << "-";
          firstShortOption = false;
        }

        usageDesc << it->optionDisplayName_[1];
      }

    }
    usageDesc << "] ";

    /** simple flags that DO NOT have a short version
     */
    for (std::vector<rad::CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( ! it->hasShort_ && ! it->hasArgument_ && ! it->required_ )
      {
        usageDesc << "[" << it->optionDisplayName_ << "] ";
      }

    }

    /** options with arguments
     */
    for (std::vector<rad::CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( it->hasArgument_ && ! it->required_ )
      {
        usageDesc << "[" << it->optionDisplayName_ << " ARG] ";
      }

    }

    /** required options with arguments
     */
    for (std::vector<rad::CustomOptionDescription>::iterator it = options_.begin();
         it != options_.end();
         ++it)
    {
      if ( it->hasArgument_ && it->required_ )
      {
        usageDesc << it->optionDisplayName_ << " ARG ";
      }

    }

    /** positional option
     */
    for (std::vector<rad::CustomOptionDescription>::iterator it = positionalOptions_.begin();
         it != positionalOptions_.end();
         ++it)
    {
      usageDesc << it->optionDisplayName_ << " ";

    }

    return usageDesc.str();

  }

//---------------------------------------------------------------------------------------------------------------------
  void OptionPrinter::printStandardAppDesc(const std::string& appName,
                                           std::ostream& out,
                                           boost::program_options::options_description desc,
                                           boost::program_options::positional_options_description* positionalDesc)
  {
    rad::OptionPrinter optionPrinter;

    typedef std::vector<boost::shared_ptr<boost::program_options::option_description > > Options;
    Options allOptions = desc.options();
    for (Options::iterator it = allOptions.begin();
         it != allOptions.end();
         ++it)
    {
      rad::CustomOptionDescription currOption(*it);
      if ( positionalDesc )
      {
        currOption.checkIfPositional(*positionalDesc);
      }

      optionPrinter.addOption(currOption);

    } // foreach option

    out << "USAGE: " << appName << " " << optionPrinter.usage() << std::endl;

  }

//---------------------------------------------------------------------------------------------------------------------
  void OptionPrinter::formatRequiredOptionError(boost::program_options::required_option& error)
  {
    std::string currOptionName = error.get_option_name();

    std::size_t idx = currOptionName.find_first_not_of("-");
    currOptionName = currOptionName.substr(idx);

    error.set_option_name(currOptionName);

  }

//---------------------------------------------------------------------------------------------------------------------

} // namespace
