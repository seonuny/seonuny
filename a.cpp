
void check_args(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

#define FMT_PRINTF(...) \
  if (false) check_args(__VA_ARGS__); \
  fmt::printf(__VA_ARGS__);

int main() {
  try {
    FMT_PRINTF("%s", 42);
  } catch (const std::exception &e) {
    fmt::print("error: {}\n", e.what());
  }
}
