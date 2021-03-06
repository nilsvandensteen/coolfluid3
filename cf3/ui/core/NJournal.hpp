// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_ui_core_NJournal_hpp
#define cf3_ui_core_NJournal_hpp

/////////////////////////////////////////////////////////////////////////////

#include <QObject>

#include "ui/core/CNode.hpp"

/////////////////////////////////////////////////////////////////////////////

class QString;

/////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace ui {
namespace core {

/////////////////////////////////////////////////////////////////////////////

class NJournal : public QObject, public CNode
{
  Q_OBJECT

public: // typedefs

  
  

public:

  NJournal(const std::string & name);

  /// @name SIGNALS
  //@{

  void list_journal(common::SignalArgs & node);

  //@} END SIGNALS

  /// @name VIRTUAL FUNCTIONS
  //@{

  /// @brief Gives the tool tip text
  /// @return Returns The class name
  virtual QString tool_tip() const;

  //@} END VIRTUAL FUNCTIONS

signals:

  void journal_request(bool local);

protected:

  /// Disables the local signals that need to.
  /// @param localSignals Map of local signals. All values are set to true
  /// by default.
  virtual void disable_local_signals(QMap<QString, bool> & localSignals) const {}

}; // NJournal

/////////////////////////////////////////////////////////////////////////////

class JournalNotifier : public QObject
{
  Q_OBJECT

public:

  static JournalNotifier & instance();

  void regist(const NJournal* journal);

signals:

  void journal_request(bool local);

private:

  JournalNotifier();

}; // JournalNotifier

/////////////////////////////////////////////////////////////////////////////

} // Core
} // ui
} // cf3

/////////////////////////////////////////////////////////////////////////////

#endif // cf3_ui_core_NJournal_hpp

