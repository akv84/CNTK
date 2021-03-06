//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#pragma once
#include <opencv2/core/mat.hpp>
#include "Config.h"
#ifdef USE_ZIP
#include <zip.h>
#include <unordered_map>
#include <memory>
#include "ConcStack.h"
#endif

namespace Microsoft { namespace MSR { namespace CNTK {

class ByteReader
{
public:
    ByteReader() = default;
    virtual ~ByteReader() = default;

    virtual void Register(size_t seqId, const std::string& path) = 0;
    virtual cv::Mat Read(size_t seqId, const std::string& path, bool grayscale) = 0;

    DISABLE_COPY_AND_MOVE(ByteReader);
};

class FileByteReader : public ByteReader
{
public:
    void Register(size_t, const std::string&) override {}
    cv::Mat Read(size_t seqId, const std::string& path, bool grayscale) override;
};

#ifdef USE_ZIP
class ZipByteReader : public ByteReader
{
public:
    ZipByteReader(const std::string& zipPath);

    void Register(size_t seqId, const std::string& path) override;
    cv::Mat Read(size_t seqId, const std::string& path, bool grayscale) override;

private:
    using ZipPtr = std::unique_ptr<zip_t, void(*)(zip_t*)>;
    ZipPtr OpenZip();

    std::string m_zipPath;
    conc_stack<ZipPtr> m_zips;
    std::unordered_map<size_t, std::pair<zip_uint64_t, zip_uint64_t>> m_seqIdToIndex;
    conc_stack<std::vector<unsigned char>> m_workspace;
};
#endif

}}}
