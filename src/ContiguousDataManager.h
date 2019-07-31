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
    bool isValid() const {
        return _data != nullptr;
    }

    // Unchecked
    const METADATA_T& getMetaData() const {
        return *getMetaDataPtr();
    }

    // Unchecked
    const DATA_T& getDatum(unsigned int index) const {
        return *(getDataPtr() + index);
    }

    // Unchecked
    inline Size_T numDataPoints() const {
        return *getSizeIntPtr();
    }

    // Calculates how much space would be required for a contiguous data structure
    static std::size_t requiredBytes(std::size_t numData) {
        return sizeof(Size_T) + sizeof(METADATA_T) + sizeof(DATA_T) * numData;
    }
private:
    void set(const METADATA_T& srcMetadata, const std::vector<DATA_T>& srcData) {
        *getSizeIntPtr() = srcData.size();
        *getMetaDataPtr() = srcMetadata;

        DATA_T * datumPtr = getDataPtr();
        for(const DATA_T& srcDatum: srcData) {
            *datumPtr = srcDatum;
            ++datumPtr;
        }
    }

    Size_T * getSizeIntPtr() {
        return reinterpret_cast<Size_T*>(_data);
    }
    const Size_T * getSizeIntPtr() const {
        return reinterpret_cast<const Size_T*>(_data);
    }

    METADATA_T * getMetaDataPtr() {
        return reinterpret_cast<METADATA_T*>(getSizeIntPtr() + 1);
    }
    const METADATA_T * getMetaDataPtr() const {
        return reinterpret_cast<const METADATA_T*>(getSizeIntPtr() + 1);
    }

    // Returns a pointer to the first datum
    DATA_T * getDataPtr() {
        return reinterpret_cast<DATA_T*>(getMetaDataPtr() + 1);
    }
    const DATA_T * getDataPtr() const {
        return reinterpret_cast<const DATA_T*>(getMetaDataPtr() + 1);
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



//template<typename DATA_T, typename METADATA_T = NoMetaData>
//class ContiguousDataManagerTestChild: public ContiguousDataManager<DATA_T, METADATA_T> {
//
//};





#endif //FASTCOLLISION_CONTIGUOUSDATAMANAGER_H