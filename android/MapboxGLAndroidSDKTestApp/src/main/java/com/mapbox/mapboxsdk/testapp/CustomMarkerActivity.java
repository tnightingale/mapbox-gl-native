package com.mapbox.mapboxsdk.testapp;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;

import com.mapbox.mapboxsdk.annotations.MarkerOptions;
import com.mapbox.mapboxsdk.annotations.Sprite;
import com.mapbox.mapboxsdk.annotations.SpriteFactory;
import com.mapbox.mapboxsdk.constants.Style;
import com.mapbox.mapboxsdk.geometry.LatLng;
import com.mapbox.mapboxsdk.utils.ApiAccess;
import com.mapbox.mapboxsdk.views.MapView;

public class CustomMarkerActivity extends AppCompatActivity {

    private MapView mapView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_custom_marker);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setDisplayShowHomeEnabled(true);
        }

        mapView = (MapView) findViewById(R.id.mapView);
        mapView.setAccessToken(ApiAccess.getToken(this));
        mapView.onCreate(savedInstanceState);

        SpriteFactory spriteFactory = new SpriteFactory(mapView);
        Drawable drawable = ContextCompat.getDrawable(this, R.drawable.ic_beenhere_black_18dp);

        // The order these sprites are created matters; the first sprite doesn't render.
        Sprite beenHereIcon = spriteFactory.fromDrawable(drawable);
        Sprite arrowIcon = spriteFactory.fromAsset("ic_arrow_drop_down_black_24dp.png");
        Sprite rocketIcon = spriteFactory.fromAsset("rocket-24@2x.png");

        mapView.addMarker(new MarkerOptions()
                .position(new LatLng(0.0, 0.0))
                .title("Default marker"));
        mapView.addMarker(new MarkerOptions()
                .position(new LatLng(0.0, 0.2))
                .title("Rocket marker")
                .icon(rocketIcon));
        mapView.addMarker(new MarkerOptions()
                .position(new LatLng(0.0, 0.4))
                .title("Arrow marker")
                .icon(arrowIcon));
        mapView.addMarker(new MarkerOptions()
                .position(new LatLng(0.0, 0.6))
                .title("Been here marker")
                .icon(beenHereIcon));
        mapView.addMarker(new MarkerOptions()
                .position(new LatLng(0.0, 0.8))
                .title("Second arrow marker")
                .icon(arrowIcon));
        mapView.addMarker(new MarkerOptions()
                .position(new LatLng(0.0, 1))
                .title("Second default marker"));
    }

    @Override
    protected void onStart() {
        super.onStart();
        mapView.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mapView.onStop();
    }

    @Override
    public void onPause()  {
        super.onPause();
        mapView.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();
        mapView.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mapView.onDestroy();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        mapView.onSaveInstanceState(outState);
    }
}
