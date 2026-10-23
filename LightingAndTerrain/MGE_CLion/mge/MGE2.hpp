#ifndef MGE2_HPP
#define MGE2_HPP

#include "core/AbstractGame.hpp"

class DebugHud;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */
class MGE2 : public AbstractGame {
    //PUBLIC FUNCTIONS

public:
    MGE2();

    virtual ~MGE2();

    //override initialize so we can add a DebugHud
    void initialize() override;

protected:
    //override so we can construct the actual scene
    virtual void _initializeScene();

    //override render to render the hud as well.
    virtual void _render();

private:
    DebugHud *_hud; //hud display

    void _updateHud();

    void UpdateCamera();

    MGE2(const MGE2 &);

    MGE2 &operator=(const MGE2 &);
};

#endif // MGE2_HPP
