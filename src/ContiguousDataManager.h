//
// Created by jon on 26/07/2019.
//

#ifndef FASTCOLLISION_CONTIGUOUSDATAMANAGER_H
#define FASTCOLLISION_CONTIGUOUSDATAMANAGER_H

#include <cstdint>

template<typename T, typename U>
class ContiguousDataManager;


template<typename DATA_T, typename METADATA_T>
class ContiguousDataReference {
    friend class ContiguousDataManager<DATA_T, METADATA_T>;
public:
    typedef int32_t Size_T;

    explicit ContiguousDataReference(void* data)
        :_data(data)
    {
        // ...
    }

    // Will check that the referenced data is not nullptr.
    // Cannot verify that the data is actually valid if it isn't null.
    inline bool isValid() const {
        return _data != nullptr;
    }

    // Unchecked
    inline const METADATA_T& getMetaData() const {
        return getHeader().metadata;
    }
    inline METADATA_T& getMetaData() {
        return getHeader().metadata;
    }

    // Unchecked
    inline const DATA_T& getDatum(unsigned int index) const {
        return *(getDataPtr() + index);
    }

    // Unchecked
    inline Size_T numDataPoints() const {
        return getHeader().sizeInt;
    }

    // Calculates how much space would be required for a contiguous data structure
    inline static std::size_t requiredBytes(std::size_t numData) {
        return sizeof(Header) + sizeof(DATA_T) * numData;
    }

    inline const void * getAddress() const { return _data; }
    inline void * getAddress() { return _data; }


    // TODO: Consider making private again:
    // Returns a pointer to the first datum
    inline DATA_T * getDataPtr() {
        return reinterpret_cast<DATA_T*>(&getHeader() + 1);
    }
    inline const DATA_T * getDataPtr() const {
        return reinterpret_cast<const DATA_T*>(&getHeader() + 1);
    }
    //

private:
    struct Header {
        Size_T sizeInt;
        METADATA_T metadata;
    };


    void set(const METADATA_T& srcMetadata, const std::vector<DATA_T>& srcData) {
        getHeader().sizeInt = srcData.size();
        getHeader().metadata = srcMetadata;

        DATA_T * datumPtr = getDataPtr();
        for(const DATA_T& srcDatum: srcData) {
            *datumPtr = srcDatum;
            ++datumPtr;
        }
    }


    inline Header & getHeader() {
        return *reinterpret_cast<Header*>(_data);
    }
    inline const Header & getHeader() const {
        return *reinterpret_cast<Header*>(_data);
    }


    void * _data = nullptr;
};




/*
 * Stores data structures consisting of an variable number of variables of type DATA_T.
 * Each structure is prefaced with an integer representing the number of DATA_T variables followed by a
 * structure of arbitrary metadata.
 * All the data is guaranteed to be contiguous
 */
template<typename DATA_T, typename METADATA_T>
class ContiguousDataManager {
public:
    explicit ContiguousDataManager(std::size_t bytesAllotment)

    {
        _data = reinterpret_cast<char*>(malloc(bytesAllotment));
        if(_data){
            _dataEnd = _data;
            _dataSize = bytesAllotment;
        }
        else {
            // TODO: Handle Error?
        }
    }

    ~ContiguousDataManager() {
        free(_data);
    }

    ContiguousDataReference<DATA_T, METADATA_T> add(const METADATA_T& metadata, const std::vector<DATA_T>& data) {
        const std::size_t prospectiveObjectSize = ContiguousDataReference<DATA_T, METADATA_T>::requiredBytes(data.size());
        char * const prospectiveObjectEnd = _dataEnd + prospectiveObjectSize;
        if(prospectiveObjectEnd <= capacityEnd()) {
            ContiguousDataReference<DATA_T, METADATA_T> newObject{_dataEnd};
            newObject.set(metadata, data);
            _dataEnd = prospectiveObjectEnd;
            return newObject;
        }
        // Insufficient capacity to create specified object.
        return ContiguousDataReference<DATA_T, METADATA_T>(nullptr);
    }

    // Get a reference to the next contiguous data object after the specified one. Cannot check the validity of the input
    // reference. Will return an invalid reference if the input reference is the last object.
    inline ContiguousDataReference<DATA_T, METADATA_T> next(const ContiguousDataReference<DATA_T, METADATA_T>& reference) const {
        const auto currentObjBytes = ContiguousDataReference<DATA_T, METADATA_T>::requiredBytes(reference.numDataPoints());
        // The const cast is used here as we want to get a mutable reference to the next object but communicate the fact that the input
        // reference isn't modified but making it const. I consider the const cast justifiable here as we are not actually casting away the constness
        // on the input reference but rather the next object.
        char* const nextObject = const_cast<char*>(reinterpret_cast<const char*>(reference.getAddress()) + currentObjBytes);
        return nextObject < _dataEnd ? ContiguousDataReference<DATA_T, METADATA_T>(nextObject) : ContiguousDataReference<DATA_T, METADATA_T>(nullptr);
    }

    inline ContiguousDataReference<DATA_T, METADATA_T> first() {
        return _data < _dataEnd ? ContiguousDataReference<DATA_T, METADATA_T>(_data) : ContiguousDataReference<DATA_T, METADATA_T>(nullptr);
    }
private:
    char * capacityEnd() {
        return _data + _dataSize;
    }

    std::size_t _dataSize = 0;

    char * _data = nullptr;

    // This points to the next free address at the end of the queue (i.e. one past the
    // last utilized address in the data).
    char * _dataEnd = nullptr;
};


//template<typename DATA_T, typename METADATA_T>
//class ContiguousDataIterator {
//public:
//    explicit ContiguousDataIterator(void * start)
//        :_currentObject(start)
//    {
//        //...
//    }
//
//    // TODO: Just finished this. Not tested. For use in CollisionPoly::RunCollisionCheck()
//    ContiguousDataIterator<DATA_T, METADATA_T> & operator++() {
//        const auto currentObjBytes = ContiguousDataReference<DATA_T, METADATA_T>::requiredBytes(_currentObject->numDataPoints());
//        const char* nextObject = reinterpret_cast<char*>(_currentObject) + currentObjBytes;
//        _currentObject = ContiguousDataReference<DATA_T, METADATA_T>(nextObject);
//        return *this;
//    }
//private:
//    ContiguousDataReference<DATA_T, METADATA_T> * _currentObject = nullptr;
//};


#endif //FASTCOLLISION_CONTIGUOUSDATAMANAGER_H
