// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_GUI_Client_Core_NHistory_hpp
#define CF_GUI_Client_Core_NHistory_hpp

#include <boost/signals2/signal.hpp>

#include "GUI/Client/Core/CNode.hpp"

class QString;

namespace CF {
namespace GUI {
namespace ClientCore {


class CHistoryNotifier{
public:

  /// signal for the set_xy_data of graph.
  /*
  typedef boost::signals2::signal< void ( std::vector<double>&, std::vector<double>& ) >  sig_type;
  */
  typedef boost::signals2::signal< void ( std::vector< std::vector<double> > & fcts,
                                          std::vector<QString> & fct_label) >  sig_type;

  /// implementation of instance that return this ( staticly ).
  static CHistoryNotifier & instance(){
     static CHistoryNotifier inst; //create static instance
     return inst; //return the static instance
  }

  sig_type notify_history;

private:
  /// Empty constructor.
  CHistoryNotifier(){}

  /// Destructor.
  ~CHistoryNotifier(){}
};

class NHistory :
    public QObject,
    public CNode
{
  Q_OBJECT

public: //typedefs

  typedef boost::shared_ptr<NHistory> Ptr;
  typedef boost::shared_ptr<NHistory const> ConstPtr;

public:

  NHistory(const QString & name);

  virtual QString toolTip() const;

  void get_history ( Common::XmlNode& node );

  void convergence_history ( Common::XmlNode& node );

}; //  XYPlot

} // ClientCore
} // GUI
} // CF


#endif // CF_GUI_Client_Core_NHistory_hpp