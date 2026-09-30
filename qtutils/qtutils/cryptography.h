#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <QString>

#include <botan/cipher_mode.h>
#include <botan/hash.h>
#include <botan/rng.h>
#include <botan/auto_rng.h>

namespace QtUtils::Cryptography {

using ByteArray = Botan::secure_vector<uint8_t>;

inline ByteArray hash(const std::string &bytes) {
  std::unique_ptr<Botan::HashFunction> hasher(
      Botan::HashFunction::create_or_throw("SHA-256"));
  const std::vector<uint8_t> data(bytes.begin(), bytes.end());
  return hasher->process(data);
}

inline std::string encrypt(const std::string &tasks,
                           const QString &passphrase) {
  std::unique_ptr<Botan::Cipher_Mode> enc(
      Botan::Cipher_Mode::create_or_throw("AES-256/CBC/PKCS7",
                                          Botan::Cipher_Dir::Encryption));
  enc->set_key(hash(passphrase.toStdString()));

  Botan::AutoSeeded_RNG rng;

  ByteArray iv = rng.random_vec(enc->default_nonce_length());
  ByteArray ciphertext((uint8_t *)tasks.data(),
                       (uint8_t *)(tasks.data() + tasks.size()));

  enc->start(iv);
  enc->finish(ciphertext);

  std::string output;
  output.reserve(iv.size() + ciphertext.size());
  output.insert(output.end(), (char *)iv.data(), (char *)iv.data() + iv.size());
  output.insert(output.end(),
                (char *)ciphertext.data(),
                (char *)ciphertext.data() + ciphertext.size());

  return output;
}

inline std::string decrypt(const std::string &ciphertext,
                           const QString &passphrase) {
  std::unique_ptr<Botan::Cipher_Mode> enc(
      Botan::Cipher_Mode::create_or_throw("AES-256/CBC/PKCS7",
                                          Botan::Cipher_Dir::Decryption));
  enc->set_key(hash(passphrase.toStdString()));

  Botan::secure_vector<uint8_t> plaintext(
      (uint8_t *)ciphertext.data(),
      (uint8_t *)(ciphertext.data() + ciphertext.size()));

  enc->start(plaintext.data(), enc->default_nonce_length());
  enc->finish(plaintext);

  return std::string((char *)(plaintext.data() + enc->default_nonce_length()),
                     plaintext.size() - enc->default_nonce_length());
}
} // namespace QtUtils::Cryptography
