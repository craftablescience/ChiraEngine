#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>

namespace chira {
    class MonoRuntime {
        public:
            MonoRuntime() = delete;
            /// @brief Initializes the Mono Runtime
            static void init();
            /// @brief Shutsdown the Mono Runtime.
            /// @note Once shutdown, you CANNOT reinit the Mono Runtime.
            static void shutdown();

            /// @brief Returns the current MonoDomain
            /// @return MonoDomain*
            static MonoDomain *getDomain() {
                return MonoRuntime::domain;
            };

            /// @brief Returns the base ChiraSharp bindings assembly
            /// @return MonoAssembly*
            static MonoAssembly *getCEAssembly() {
                return MonoRuntime::ceassembly;
            };

        private:
            static inline MonoDomain *domain = nullptr;
            static inline MonoAssembly *ceassembly = nullptr;
    };
}