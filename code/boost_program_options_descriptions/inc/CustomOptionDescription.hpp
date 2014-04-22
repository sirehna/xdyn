/**********************************************************************************************************************
**         __________              ___                              ________                                         **
**         \______   \_____     __| _/ _____  _____     ____       /  _____/ _____     _____    ____    ______       **
**          |       _/\__  \   / __ | /     \ \__  \   /    \     /   \  ___ \__  \   /     \ _/ __ \  /  ___/       **
**          |    |   \ / __ \_/ /_/ ||  Y Y  \ / __ \_|   |  \    \    \_\  \ / __ \_|  Y Y  \\  ___/  \___ \        **
**          |____|_  /(____  /\____ ||__|_|  /(____  /|___|  /     \______  /(____  /|__|_|  / \___  \/____  \       **
**                 \/      \/      \/      \/      \/      \/             \/      \/       \/      \/      \/        **
**                                                         2012                                                      **
**********************************************************************************************************************/

#ifndef RAD_CUSTOMOPTIONDESCRIPTION_HPP
#define RAD_CUSTOMOPTIONDESCRIPTION_HPP

#include "boost/program_options.hpp"

#include <string>

namespace rad
{
//*********************************************************************************************************************
  class CustomOptionDescription
  {
  public: // interface
    CustomOptionDescription(boost::shared_ptr<boost::program_options::option_description> option);

    void checkIfPositional(const boost::program_options::positional_options_description& positionalDesc);

    std::string getOptionUsageString();

  public: // data
    std::string optionID_;
    std::string optionDisplayName_;
    std::string optionDescription_;
    std::string optionFormatName_;

    bool required_;
    bool hasShort_;
    bool hasArgument_;
    bool isPositional_;


  }; // class

//*********************************************************************************************************************

} // namespace

#endif /* CUSTOMOPTIONDESCRIPTION_HPP_ */
