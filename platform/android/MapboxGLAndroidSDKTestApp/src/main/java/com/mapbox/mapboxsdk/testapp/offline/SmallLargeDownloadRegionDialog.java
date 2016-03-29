package com.mapbox.mapboxsdk.testapp.offline;

import android.app.Activity;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.View;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import com.mapbox.mapboxsdk.constants.Style;
import com.mapbox.mapboxsdk.geometry.LatLng;
import com.mapbox.mapboxsdk.geometry.LatLngBounds;
import com.mapbox.mapboxsdk.offline.OfflineRegionDefinition;
import com.mapbox.mapboxsdk.offline.OfflineTilePyramidRegionDefinition;
import com.mapbox.mapboxsdk.testapp.R;

/**
 * Created by tnightingale on 2016-03-28.
 */
public class SmallLargeDownloadRegionDialog extends DialogFragment {

    private final static String LOG_TAG = "DownloadRegionDialog";

    public interface DownloadRegionDialogListener {
        void onDownloadRegionDialogPositiveClick(final String regionName, OfflineRegionDefinition definition);
    }

    DownloadRegionDialogListener mListener;

    String mapStyleUrl;

    float density;

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        mListener = (DownloadRegionDialogListener) activity;
        mapStyleUrl = Style.MAPBOX_STREETS;
        density = activity.getResources().getDisplayMetrics().density;
    }

    @NonNull
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());

        // Let the user choose a name for the region
        final String smallRegionName = "Small region (~74MB)";
        final String largeRegionName = "Large region (~87MB)";
        final RadioButton smallRegion = new RadioButton(getActivity());
        final RadioButton largeRegion = new RadioButton(getActivity());
        final RadioGroup group = new RadioGroup(getActivity());

        smallRegion.setId(R.id.small_region);
        smallRegion.setText(smallRegionName);
        largeRegion.setId(R.id.large_region);
        largeRegion.setText(largeRegionName);

        group.addView(smallRegion);
        group.addView(largeRegion);
        group.check(R.id.small_region);

        builder.setTitle("Choose a name for the region")
                .setIcon(R.drawable.ic_airplanemode_active_black_24dp)
                .setView(group)
                .setPositiveButton("Start", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        int checkedId = group.getCheckedRadioButtonId();
                        String regionName = checkedId == R.id.small_region ?
                                smallRegionName :
                                largeRegionName;
                        OfflineRegionDefinition definition = checkedId == R.id.small_region ?
                                getSmallRegionDefinition() :
                                getLargeRegionDefinition();
                        mListener.onDownloadRegionDialogPositiveClick(regionName, definition);
                    }
                }).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Log.d(LOG_TAG, "Download cancelled.");
            }
        });

        return builder.create();
    }

    protected OfflineRegionDefinition getLargeRegionDefinition() {
        double north = -41.219;
        double east = 174.927;
        double south = -41.348;
        double west = 174.733;
        double minZoom = 0;
        double maxZoom = 16;

        LatLngBounds bounds = new LatLngBounds.Builder()
                .include(new LatLng(north, west))
                .include(new LatLng(south, east))
                .build();

        // Definition
        return new OfflineTilePyramidRegionDefinition(
                mapStyleUrl, bounds, minZoom, maxZoom, density);
    }

    protected OfflineRegionDefinition getSmallRegionDefinition() {
        double north = -41.302;
        double east = 174.772;
        double south = -41.312;
        double west = 174.756;

        double minZoom = 0;
        double maxZoom = 16;

        LatLngBounds bounds = new LatLngBounds.Builder()
                .include(new LatLng(north, west))
                .include(new LatLng(south, east))
                .build();

        // Definition
        return new OfflineTilePyramidRegionDefinition(
                mapStyleUrl, bounds, minZoom, maxZoom, density);
    }
}
