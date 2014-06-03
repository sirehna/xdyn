/**********************************************************************************************************************
**         __________              ___                              ________                                         **
**         \______   \_____     __| _/ _____  _____     ____       /  _____/ _____     _____    ____    ______       **
**          |       _/\__  \   / __ | /     \ \__  \   /    \     /   \  ___ \__  \   /     \ _/ __ \  /  ___/       **
**          |    |   \ / __ \_/ /_/ ||  Y Y  \ / __ \_|   |  \    \    \_\  \ / __ \_|  Y Y  \\  ___/  \___ \        **
**          |____|_  /(____  /\____ ||__|_|  /(____  /|___|  /     \______  /(____  /|__|_|  / \___  \/____  \       **
**                 \/      \/      \/      \/      \/      \/             \/      \/       \/      \/      \/        **
**                                                         2012                                                      **
**********************************************************************************************************************/

#include "CustomOptionDescription.hpp"

#include "boost/algorithm/string/erase.hpp"

#include <iomanip>

namespace
{
  const size_t LONG_NON_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT = 0;
  const size_t LONG_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT = 1;
  const size_t SHORT_PREPENDED_IF_EXIST_ELSE_LONG = 4;

  const size_t SHORT_OPTION_STRING_LENGTH = 2; // -x
  const size_t ADEQUATE_WIDTH_FOR_OPTION_NAME = 20;

  const bool HAS_ARGUMENT = true;
  const bool DOES_NOT_HAVE_ARGUMENT = false;

} // namespace

namespace rad
{
//---------------------------------------------------------------------------------------------------------------------
  CustomOptionDescription::CustomOptionDescription(boost::shared_ptr<boost::program_options::option_description> option) :
        optionID_(""),
        optionDisplayName_(""),
        optionDescription_(""),
        optionFormatName_(""),
    required_(false),
    hasShort_(false),
    hasArgument_(false),
    isPositional_(false)
  {
    if ( (option->canonical_display_name(SHORT_PREPENDED_IF_EXIST_ELSE_LONG).size() == SHORT_OPTION_STRING_LENGTH ) )
    {
      hasShort_ = true;
      optionID_ = option->canonical_display_name(SHORT_PREPENDED_IF_EXIST_ELSE_LONG);
      optionDisplayName_ = option->canonical_display_name(SHORT_PREPENDED_IF_EXIST_ELSE_LONG);
    }
    else
    {
      hasShort_ = false;
      optionID_ = option->canonical_display_name(LONG_NON_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT);
      optionDisplayName_ = option->canonical_display_name(LONG_PREPENDED_IF_EXIST_ELSE_PREPENDED_SHORT);
    }

    boost::shared_ptr<const boost::program_options::value_semantic> semantic = option->semantic();
    required_ = semantic->is_required();
    hasArgument_ = semantic->max_tokens() > 0 ? HAS_ARGUMENT : DOES_NOT_HAVE_ARGUMENT;

    optionDescription_ = option->description();
    optionFormatName_ = option->format_name();

  }

//---------------------------------------------------------------------------------------------------------------------
  void CustomOptionDescription::checkIfPositional(const boost::program_options::positional_options_description& positionalDesc)
  {
    for (size_t i = 0; i < positionalDesc.max_total_count(); ++i)
    {
      if (optionID_ == positionalDesc.name_for_position((unsigned int)(i)))
      {
        boost::algorithm::erase_all(optionDisplayName_, "-");
        isPositional_ = true;
        break;
      }

    } // for

  }

//---------------------------------------------------------------------------------------------------------------------
  std::string CustomOptionDescription::getOptionUsageString()
  {
    std::stringstream usageString;
    if ( isPositional_ )
    {
      usageString << "\t" << std::setw(ADEQUATE_WIDTH_FOR_OPTION_NAME) << std::left << optionDisplayName_ << "\t" << optionDescription_;
    }
    else
    {
      usageString << "\t" << std::setw(ADEQUATE_WIDTH_FOR_OPTION_NAME) << std::left << optionFormatName_ << "\t" << optionDescription_;
    }

    return usageString.str();

  }

//---------------------------------------------------------------------------------------------------------------------

} // namespace
