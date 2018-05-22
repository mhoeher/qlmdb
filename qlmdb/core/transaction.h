#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QScopedPointer>
#include <QtGlobal>

namespace QLMDB {
namespace Core {

class Context;
class TransactionPrivate;

class Transaction
{
public:
    static const unsigned int ReadOnly;

    explicit Transaction(Context &context, unsigned int flags = 0);
    explicit Transaction(Transaction &parent, unsigned int flags = 0);
    virtual ~Transaction();

    Transaction& operator =(const Transaction &other) = delete;

    bool isValid() const;
    int lastError() const;
    QString lastErrorString() const;

    bool commit();
    bool abort();


private:
    QScopedPointer<TransactionPrivate> d_ptr;

    Q_DECLARE_PRIVATE(Transaction)

};

} // namespace Core
} // namespace QLMDB

#endif // TRANSACTION_H
