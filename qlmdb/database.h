/*
 * This file is part of QLMDB.
 *
 * QLMDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * QLMDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QLMDB.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DATABASE_H
#define DATABASE_H

#include <type_traits>

#include <QByteArray>
#include <QByteArrayList>
#include <QScopedPointer>
#include <QString>

#include "qlmdb_global.h"

namespace QLMDB {

class Cursor;
class Context;
class DatabasePrivate;
class Transaction;

class QLMDBSHARED_EXPORT Database
{
    friend class Cursor;
public:
    static const unsigned int ReverseKey;
    static const unsigned int MultiValues;
    static const unsigned int IntegerKeys;
    static const unsigned int FixedSizeMultiValues;
    static const unsigned int IntegerKeysMultiValues;
    static const unsigned int ReverseKeyMultiValues;
    static const unsigned int Create;

    explicit Database(Context &context,
             const QString &name = QString(),
             unsigned int flags = Create);
    explicit Database(Transaction &transaction,
             const QString &name = QString(),
             unsigned int flags = Create);
    virtual ~Database();

    bool isValid() const;
    int lastError() const;
    QString lastErrorString() const;
    void clearLastError();

    bool put(const QByteArray &key, const QByteArray &value);
    bool put(QLMDB::Transaction &transaction, const QByteArray &key,
             const QByteArray &value);
    QByteArray get(const QByteArray &key);
    QByteArray get(Transaction &transaction, const QByteArray &key);
    QByteArrayList getAll(const QByteArray &key);
    QByteArrayList getAll(Transaction &transaction,
                          const QByteArray &key);
    inline QByteArray operator [](const QByteArray &key);
    bool remove(const QByteArray &key);
    bool remove(Transaction &transaction, const QByteArray &key);
    bool remove(const QByteArray &key, const QByteArray &value);
    bool remove(Transaction &transaction, const QByteArray &key,
                const QByteArray &value);
    bool clear();
    bool clear(Transaction &txn);
    bool drop();
    bool drop(Transaction &transaction);

    template<typename T>
    inline bool put(
            typename std::enable_if<std::is_integral<T>::value, T>::type key,
            const QByteArray &value);

    template<typename T>
    inline bool put(
            Transaction &transaction,
            typename std::enable_if<std::is_integral<T>::value, T>::type key,
            const QByteArray &value);

    template<typename T>
    inline QByteArray get(
            typename std::enable_if<std::is_integral<T>::value, T>::type key);

    template<typename T>
    inline QByteArray get(
            Transaction &transaction,
            typename std::enable_if<std::is_integral<T>::value, T>::type key);

    template<typename T>
    inline QByteArrayList getAll(
            typename std::enable_if<std::is_integral<T>::value, T>::type key);

    template<typename T>
    inline QByteArrayList getAll(
            Transaction &transaction,
            typename std::enable_if<std::is_integral<T>::value, T>::type key);

    template<typename T>
    inline bool remove(
            typename std::enable_if<std::is_integral<T>::value, T>::type key);

    template<typename T>
    inline bool remove(
            Transaction &transaction,
            typename std::enable_if<std::is_integral<T>::value, T>::type key);

    template<typename T>
    inline bool remove(
            typename std::enable_if<std::is_integral<T>::value, T>::type key,
            const QByteArray &value);

    template<typename T>
    inline bool remove(
            Transaction &transaction,
            typename std::enable_if<std::is_integral<T>::value, T>::type key,
            const QByteArray &value);

private:

    QScopedPointer<DatabasePrivate> d_ptr;
    Q_DECLARE_PRIVATE(Database)

};



/**
 * @brief Get the value for the given @p key from the database.
 *
 * This is basically equivalent to calling the get() method.
 *
 * @note This method must not be called when another Transaction is
 * active in the same thread.
 */
QByteArray Database::operator [](const QByteArray &key)
{
    return get(key);
}


/**
 * @brief Insert the @p key - @p value pair into the database.
 *
 * This is a convenience method which allows to use integral types
 * as key.
 *
 * @note This method must not be called when another Transaction is
 * active in the same thread.
 */
template<typename T>
bool Database::put(
        typename std::enable_if<std::is_integral<T>::value, T>::type key,
        const QByteArray &value)
{
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return put(k, value);
}


/**
 * @brief Insert the @p key - @p value pair into the database.
 *
 * This is an overloaded version of put() which runs the
 * operation in the given @p transaction.
 */
template<typename T>
bool Database::put(
        Transaction &transaction,
        typename std::enable_if<std::is_integral<T>::value, T>::type key,
        const QByteArray &value) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return put(transaction, k, value);
}


/**
 * @brief Get the value for the given @p key from the database.
 *
 * This is a convenience method which allows to use integral types
 * as key.
 *
 * @note This method must not be called when another Transaction is
 * active in the same thread.
 */
template<typename T>
QByteArray Database::get(
        typename std::enable_if<std::is_integral<T>::value, T>::type key) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return get(k);
}


/**
 * @brief Get the value for the given @p key from the database.
 *
 * This is an overloaded version of get() which runs the
 * operation in the given @p transaction.
 */
template<typename T>
inline QByteArray Database::get(
        Transaction &transaction,
        typename std::enable_if<std::is_integral<T>::value, T>::type key) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return get(transaction, k);
}


/**
 * @brief Get all values for the given @p key from the database.
 *
 * This is a convenience method which allows to use integral types
 * as key.
 *
 * @note This method must not be called when another Transaction is
 * active in the same thread.
 */
template<typename T>
QByteArrayList Database::getAll(
        typename std::enable_if<std::is_integral<T>::value, T>::type key) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return  getAll(k);
}


/**
 * @brief Get all values for the given @p key from the database.
 *
 * This is an overloaded version of getAll() which runs the
 * operation in the given @p transaction.
 */
template<typename T>
inline QByteArrayList Database::getAll(
        Transaction &transaction,
        typename std::enable_if<std::is_integral<T>::value, T>::type key) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return getAll(transaction, k);
}


/**
 * @brief Remove all values for the given @p key.
 *
 * This is a convenience method which allows to use integral types
 * as key.
 *
 * @note This method must not be called when another Transaction is
 * active in the same thread.
 */
template<typename T>
bool Database::remove(
        typename std::enable_if<std::is_integral<T>::value, T>::type key) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return remove(k);
}


/**
 * @brief Remove all values for the given @p key.
 *
 * This is an overloaded version of remove() which runs the
 * operation in the given @p transaction.
 */
template<typename T>
inline bool Database::remove(
        Transaction &transaction,
        typename std::enable_if<std::is_integral<T>::value, T>::type key) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return remove(transaction, k);
}


/**
 * @brief Remove a specific @p value for the given @p key.
 *
 * This is a convenience method which allows to use integral types
 * as key.
 *
 * @note This method must not be called when another Transaction is
 * active in the same thread.
 */
template<typename T>
bool Database::remove(
        typename std::enable_if<std::is_integral<T>::value, T>::type key,
        const QByteArray &value) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return remove(k, value);
}


/**
 * @brief Remove a specific @p value for the given @p key.
 *
 * This is an overloaded version of remove() which runs the
 * operation in the given @p transaction.
 */
template<typename T>
inline bool Database::remove(
        Transaction &transaction,
        typename std::enable_if<std::is_integral<T>::value, T>::type key,
        const QByteArray &value) {
    QByteArray k(reinterpret_cast<const char*>(&key), sizeof(key));
    return remove(transaction, k, value);
}


} // namespace QLMDB

#endif // DATABASE_H
