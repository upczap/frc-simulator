/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2011. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include "ErrorBase.h"
#include "NetworkTables/NetworkTableChangeListener.h"
#include "Task.h"
#include <map>
#include <semLib.h>
#include <string>
#include <vector>

/**
 * The preferences class provides a relatively simple way to save important values to
 * the cRIO to access the next time the cRIO is booted.
 *
 * <p>This class loads and saves from a file
 * inside the cRIO.  The user can not access the file directly, but may modify values at specific
 * fields which will then be saved to the file when {@link Preferences#Save() Save()} is called.</p>
 *
 * <p>This class is thread safe.</p>
 *
 * <p>This will also interact with {@link NetworkTable} by creating a table called "Preferences" with all the
 * key-value pairs.  To save using {@link NetworkTable}, simply set the boolean at position "~S A V E~" to true.
 * Also, if the value of any variable is " in the {@link NetworkTable}, then that represents non-existence in the
 * {@link Preferences} table</p>
 */
class Preferences : public ErrorBase, public NetworkTableChangeListener
{
public:
    static Preferences *GetInstance();

    std::vector<std::string> GetKeys();
    std::string GetString(const char *key, const char *defaultValue = "");
    int GetString(const char *key, char *value, int valueSize, const char *defaultValue = "");
    int GetInt(const char *key, int defaultValue = 0);
    double GetDouble(const char *key, double defaultValue = 0.0);
    float GetFloat(const char *key, float defaultValue = 0.0);
    bool GetBoolean(const char *key, bool defaultValue = false);
    INT64 GetLong(const char *key, INT64 defaultValue = 0);
    void PutString(const char *key, const char *value);
    void PutInt(const char *key, int value);
    void PutDouble(const char *key, double value);
    void PutFloat(const char *key, float value);
    void PutBoolean(const char *key, bool value);
    void PutLong(const char *key, INT64 value);
    void Save();
    bool ContainsKey(const char *key);
    void Remove(const char *key);

protected:
    Preferences();
    virtual ~Preferences();

private:
    std::string Get(const char *key);
    void Put(const char *key, std::string value);

    void ReadTaskRun();
    void WriteTaskRun();

    virtual void ValueChanged(NetworkTable *table, const char *name, NetworkTables_Types type);
    virtual void ValueConfirmed(NetworkTable *table, const char *name, NetworkTables_Types type);

    static int InitReadTask(Preferences *obj) {obj->ReadTaskRun();return 0;}
    static int InitWriteTask(Preferences *obj) {obj->WriteTaskRun();return 0;}

    static Preferences *_instance;

    /** The semaphore for accessing the file */
    SEM_ID m_fileLock;
    /** The semaphore for beginning reads and writes to the file */
    SEM_ID m_fileOpStarted;
    /** The semaphore for reading from the table */
    SEM_ID m_tableLock;
    typedef std::map<std::string, std::string> StringMap;
    /** The actual values (String->String) */
    StringMap m_values;
    /** The keys in the order they were read from the file */
    std::vector<std::string> m_keys;
    /** The comments that were in the file sorted by which key they appeared over (String->Comment) */
    StringMap m_comments;
    /** The comment at the end of the file */
    std::string m_endComment;
    Task m_readTask;
    Task m_writeTask;
};

#endif
