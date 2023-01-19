#pragma once

#include <vector>

class SystemRegistry {
public:
    struct SystemBase {
        virtual ~SystemBase() = default;
        virtual void systemInit() = 0;
        virtual void systemUpdate() = 0;
        virtual void systemDeinit() = 0;
    };

    SystemRegistry() = delete;

    static std::vector<SystemBase*>& get() {
        static std::vector<SystemBase*> systems;
        return systems;
    }

    template<typename T>
    static std::size_t add() {
        auto& systems = SystemRegistry::get();
        systems.push_back(new T{});
        return systems.size() - 1;
    }

    static void remove(std::size_t index) {
        delete SystemRegistry::get().at(index);
        SystemRegistry::get()[index] = nullptr;
    }

    static inline void initAll() {
        SystemRegistry::callOnAll(&SystemBase::systemInit);
    }

    static inline void updateAll() {
        SystemRegistry::callOnAll(&SystemBase::systemUpdate);
    }

    static inline void deinitAll() {
        SystemRegistry::callOnAll(&SystemBase::systemDeinit);
    }

private:
    static void callOnAll(void(SystemBase::*memberFunc)()) {
        for (const auto system : SystemRegistry::get()) {
            if (system) {
                (system->*memberFunc)();
            }
        }
    }
};

template<typename T>
struct System : public SystemRegistry::SystemBase {
    System() = default;

    void systemInit() final {
        if constexpr (requires {T::init();}) {
            T::init();
        } else if constexpr (requires (T t) {t.init();}) {
            static_cast<T*>(this)->init();
        }
    }

    void systemUpdate() final {
        if constexpr (requires {T::update();}) {
            T::update();
        } else if constexpr (requires (T t) {t.update();}) {
            static_cast<T*>(this)->update();
        }
    }

    void systemDeinit() final {
        if constexpr (requires {T::deinit();}) {
            T::deinit();
        } else if constexpr (requires (T t) {t.deinit();}) {
            static_cast<T*>(this)->deinit();
        }
        SystemRegistry::remove(T::registryLocation);
    }

protected:
    static inline const std::size_t registryLocation = SystemRegistry::add<System<T>>();
};

#define CHIRA_CREATE_SYSTEM(name) struct name##System : public System<name##System>
