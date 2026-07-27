
def _build_for(file, variant: int) -> None:
    file.write(f"#define ENUM_FOREACH_{variant}(type) \\\n")

    for i in range(variant):
        file.write(f"    ENUM_BUILD_ENUMERATOR(type, {i}) \\\n")

    file.write("\n")

def main(variants: list[int]) -> None:
    with open("enum-foreach.hpp", "w") as file:
        file.write("#ifndef NTSFKA_ENUM_STRING_ENUM_FOREACH_HPP\n")
        file.write("#define NTSFKA_ENUM_STRING_ENUM_FOREACH_HPP\n")
        file.write("\n")

        for variant in variants:
            _build_for(file, variant)

        file.write("\n")
        file.write("#endif // NTSFKA_ENUM_STRING_ENUM_FOREACH_HPP\n")

if __name__ == "__main__":
    main([2, 4, 8, 12, 16, 24, 32, 64, 128, 256, 512, 1024])
