#include <iostream>
#include <../include/leveldb/db.h>
#include <../include/leveldb/write_batch.h>
#include <cassert>

int main(int argc, char *argv[])
{
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb",  &db);
    assert(status.ok());
    leveldb::WriteOptions WriteOptions;
    db->Put(WriteOptions, "1", "raohui");
    db->Put(WriteOptions, "2", "test");
    db->Put(WriteOptions, "3", "leiha");
    leveldb::ReadOptions ReadOptions;
    leveldb::Iterator *it = db->NewIterator(ReadOptions);
    for(it->SeekToFirst();it->Valid();it->Next()){
        std::cout << "key:" << it->key().ToString() << "---value:" << it->value().ToString() << std::endl;
    }

    delete db;
    return 0;
}
