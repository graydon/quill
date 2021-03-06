#include "quill/detail/misc/Utilities.h"

#include "quill/detail/misc/Macros.h"
#include <codecvt>
#include <locale>
#include <system_error>

namespace quill
{
namespace detail
{
/***/
void fwrite_fully(void const* ptr, size_t size, size_t count, FILE* stream)
{
  size_t const written = std::fwrite(ptr, size, count, stream);

  if (QUILL_UNLIKELY(written < count))
  {
    throw std::system_error(errno, std::system_category());
  }
}

/***/
std::wstring s2ws(std::string const& str) noexcept
{
  using convert_t = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_t, wchar_t> converter;

  return converter.from_bytes(str);
}

/***/
std::string ws2s(std::wstring const& wstr) noexcept
{
  using convert_t = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_t, wchar_t> converter;

  return converter.to_bytes(wstr);
}

} // namespace detail
} // namespace quill