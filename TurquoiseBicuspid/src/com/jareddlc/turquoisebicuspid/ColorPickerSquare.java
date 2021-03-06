package com.jareddlc.turquoisebicuspid;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ComposeShader;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Shader;
import android.graphics.Shader.TileMode;
import android.util.AttributeSet;
import android.view.View;

public class ColorPickerSquare extends View {
	Paint paint;
	Shader shader;
	final float[] color = {1.f, 1.f, 1.f};

	public ColorPickerSquare(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public ColorPickerSquare(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
	@SuppressLint("DrawAllocation")
	@Override protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		
		if(paint == null) {
			paint = new Paint();
			shader = new LinearGradient(0.f, 0.f, 0.f, this.getMeasuredHeight(), 0xffffffff, 0xff000000, TileMode.CLAMP);
		}
		
		int rgb = Color.HSVToColor(color);
		Shader sShader = new LinearGradient(0.f, 0.f, this.getMeasuredWidth(), 0.f, 0xffffffff, rgb, TileMode.CLAMP);
		ComposeShader cShader = new ComposeShader(shader, sShader, PorterDuff.Mode.MULTIPLY);
		paint.setShader(cShader);
		canvas.drawRect(0.f, 0.f, this.getMeasuredWidth(), this.getMeasuredHeight(), paint);
	}

	void setHue(float hue) {
		color[0] = hue;
		invalidate();
	}
}
