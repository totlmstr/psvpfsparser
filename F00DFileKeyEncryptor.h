#pragma once

#include "LocalFilesystem.h"
#include "IF00DKeyEncryptor.h"

#include <map>

class F00DFileKeyEncryptor : public IF00DKeyEncryptor
{
private:
   fs::path m_filePath;

   std::map<std::string, std::string> m_keyCache;
   bool m_isCacheLoaded;

public:
   F00DFileKeyEncryptor(fs::path filePath);

private:
   int load_cache_flat_file();

public:
   int encrypt_key(const unsigned char* key, int key_size, unsigned char* drv_key) override;

   void print_cache(std::ostream& os, std::string sep = "\t") const override;
};