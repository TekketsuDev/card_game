Este zip contiene una reestructuración básica de tu proyecto de cartas con Raylib.

Estructura:

  src/
    main.c                      -> loop principal limpio
    game/GameContext.*          -> estado global del juego (mazo, mano, zonas, partículas, state machine)
    game/GameLogic.*            -> lógica de juego (usa StateMachine)
    input/InputSystem.*         -> manejo de input (drag & drop de cartas)
    render/AnimationSystem.*    -> sistema de partículas / animaciones
    render/Renderer.*           -> todo el dibujado
    state/state_machine.*       -> copiado de tu implementación actual
    objects/*.c                 -> tus ficheros originales (card, zone, card_animation, main_original)

Pasos recomendados:

  1. Copia tus headers reales (card.h, zone.h, card_animation.h, etc.) a src/objects/ o ajusta los includes.
  2. Compila usando main.c como punto de entrada.
  3. Poco a poco migra lógica desde objects/main_original.c a los módulos:
        - InputSystem (input)
        - GameLogic (game)
        - AnimationSystem (render)
        - Renderer (render)
  4. La StateMachine queda ahora desacoplada del dibujo; solo maneja reglas del juego.
