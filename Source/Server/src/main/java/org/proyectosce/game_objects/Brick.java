package org.proyectosce.game_objects;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;

public class Brick {
    private Vector2 position;
    private int points;
    private boolean addLife;
    private boolean addBall;
    private boolean addDoubleRacket;
    private boolean addHalfRacket;
    private boolean speedUp;
    private boolean speedDown;

    // Getters y setters para position
    public Vector2 getPosition() {
        return position;
    }

    public void setPosition(Vector2 position) {
        this.position = position;
    }

    // Getters y setters para points
    public int getPoints() {
        return points;
    }

    public void setPoints(int points) {
        this.points = points;
    }

    // Getters y setters para addLife
    public boolean isAddLife() {
        return addLife;
    }

    public void setAddLife(boolean addLife) {
        this.addLife = addLife;
    }

    // Getters y setters para addBall
    public boolean isAddBall() {
        return addBall;
    }

    public void setAddBall(boolean addBall) {
        this.addBall = addBall;
    }

    // Getters y setters para addDoubleRacket
    public boolean isAddDoubleRacket() {
        return addDoubleRacket;
    }

    public void setAddDoubleRacket(boolean addDoubleRacket) {
        this.addDoubleRacket = addDoubleRacket;
    }

    // Getters y setters para addHalfRacket
    public boolean isAddHalfRacket() {
        return addHalfRacket;
    }

    public void setAddHalfRacket(boolean addHalfRacket) {
        this.addHalfRacket = addHalfRacket;
    }

    // Getters y setters para speedUp
    public boolean isSpeedUp() {
        return speedUp;
    }

    public void setSpeedUp(boolean speedUp) {
        this.speedUp = speedUp;
    }

    // Getters y setters para speedDown
    public boolean isSpeedDown() {
        return speedDown;
    }

    public void setSpeedDown(boolean speedDown) {
        this.speedDown = speedDown;
    }

    public ObjectNode toJson(ObjectMapper mapper) {
        ObjectNode brickJson = mapper.createObjectNode();
        brickJson.put("position", position.toString());
        if (addLife) brickJson.put("power", "add_life");
        if (addBall) brickJson.put("power", "add_ball");
        if (addDoubleRacket) brickJson.put("power", "double_racket_size");
        if (addHalfRacket) brickJson.put("power", "half_racket_size");
        if (speedUp) brickJson.put("power", "add_speed");
        if (speedDown) brickJson.put("power", "reduce_speed");
        return brickJson;
    }
}

